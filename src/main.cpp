/*
██╗    ██╗██╗███████╗██╗██╗██████╗
██║    ██║██║██╔════╝██║██║██╔══██╗
██║ █╗ ██║██║█████╗  ██║██║██████╔╝
██║███╗██║██║██╔══╝  ██║██║██╔══██╗
╚███╔███╔╝██║██║     ██║██║██║  ██║
 ╚══╝╚══╝ ╚═╝╚═╝     ╚═╝╚═╝╚═╝  ╚═╝
*/

//Controlador IR via internet
//
//(c) JMGK 2021

#include "main.h"

const uint16_t kIrLedPin = IR_LED_PIN;
IRsend irsend(kIrLedPin);

//irRecv vars
const uint16_t kRecvPin = IR_RECV_PIN;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);
bool irin_enable = false;
bool irin_timeout;

//
IrResult irresult;

std::vector<IrResult> ir_codes;

WiFiManager wm;
ESP8266WebServer server;
#ifdef SUPPORT_OTA
ESP8266HTTPUpdateServer httpUpdater;
#endif

bool decoding_onoff;

//led blink vars
#define IR_TIMEOUT 1000 * 60 * 3
#define LED_BLINK_INTERVAL 500
unsigned long old_millis = 0;
unsigned long ir_started;
int led_status = LOW;

//device name
String wifiir_subname;

/*
███████╗███████╗████████╗██╗   ██╗██████╗
██╔════╝██╔════╝╚══██╔══╝██║   ██║██╔══██╗
███████╗█████╗     ██║   ██║   ██║██████╔╝
╚════██║██╔══╝     ██║   ██║   ██║██╔═══╝
███████║███████╗   ██║   ╚██████╔╝██║
╚══════╝╚══════╝   ╚═╝    ╚═════╝ ╚═╝
*/

void setup()
{
  WiFi.mode(WIFI_STA);

  Serial.begin(115200);
  Serial.println("");
  Serial.println("WIFIIR - Starting");

#ifdef DEBUG
  Serial.setDebugOutput(true);
  wm.setDebugOutput(true);
#else
  Serial.setDebugOutput(false);
  wm.setDebugOutput(false);
#endif

#ifdef DEBUG_ESP
  dump_esp8266();
#endif

  if (!wm.autoConnect("WIFIIR"))
  {
#ifdef DEBUG
    Serial.println("Failed to connect");
#endif
    ESP.restart();
    delay(1000);
  }
  else
  {
#ifdef DEBUG
    Serial.println("Connected...");
#endif
  }

#ifdef SUPPORT_OTA
  httpUpdater.setup(&server, "/update");
#endif

  install_www_handlers();

  server.begin();

  //blinking LED setup
  decoding_onoff = false;
  pinMode(LED_PIN, OUTPUT);
  led_status = LOW;
  digitalWrite(LED_PIN, led_status);

#ifdef SUPPORT_LITTLEFS
  if (!LittleFS.begin())
#else
  if (!SPIFFS.begin())
#endif
  {
#ifdef DEBUG
    Serial.println("An Error has occurred while mounting LittleFS");
#endif
  }

#ifdef DEBUG_ESP
  dump_fs();
#endif

  //discovery protocols
  wifiir_name_load();
  String s = "WIFIIR" + (wifiir_subname.isEmpty() ? "" : ("-" + wifiir_subname));
  Serial.printf("Net name: %s\n", s.c_str());

#ifdef SUPPORT_MDNS
  if (MDNS.begin(s.c_str()))
  {
#ifdef DEBUG
    Serial.println("MDNS OK");
#endif
  }
  else
  {
#ifdef DEBUG
    Serial.println("MDNS NOK");
#endif
  }
  MDNS.addService("http", "tcp", 80);
#endif

#ifdef SUPPORT_NETBIOS
  if (NBNS.begin(s.c_str()))
  {
#ifdef DEBUG
    Serial.println("NETBIOS OK");
#endif
  }
  else
  {
#ifdef DEBUG
    Serial.println("NETBIOS NOK");
#endif
  }
#endif

#ifdef SUPPORT_LLMNR
  if (LLMNR.begin(s.c_str()))
  {
#ifdef DEBUG
    Serial.println("LLMNR OK");
#endif
  }
  else
  {
#ifdef DEBUG
    Serial.println("LLMNR NOK");
#endif
  }
#endif
#ifdef SUPPORT_SSDP
  SSDP_esp8266.setName(s.c_str());
  SSDP_esp8266.setDeviceType("urn:schemas-upnp-org:device:WIFIIR:1");
  SSDP_esp8266.setSchemaURL("description.xml");
  SSDP_esp8266.setSerialNumber(ESP.getChipId());
  SSDP_esp8266.setURL("/");
  SSDP_esp8266.setModelName("WIFIIR");
  SSDP_esp8266.setModelNumber("1");
  SSDP_esp8266.setManufacturer("JMGK");
  SSDP_esp8266.setManufacturerURL("http://www.jmgk.com.br/");
#ifdef DEBUG
  Serial.println("SSDP OK");
#endif
#endif

  //carrega botões salvos
  codes_load();

  //setup telegram bot
#ifdef SUPPORT_TELEGRAM
  telegram_load();
  telegram_users_load();
  bt_setup();
  tb_kbd();
#endif

  irsend.begin();
}

/*
██╗      ██████╗  ██████╗ ██████╗
██║     ██╔═══██╗██╔═══██╗██╔══██╗
██║     ██║   ██║██║   ██║██████╔╝
██║     ██║   ██║██║   ██║██╔═══╝
███████╗╚██████╔╝╚██████╔╝██║
╚══════╝ ╚═════╝  ╚═════╝ ╚═╝
*/

void _end_ir()
{
  irrecv.disableIRIn();
  led_status = LOW;
  digitalWrite(LED_PIN, led_status);
  irin_enable = false;
  decoding_onoff = false;
}

void loop()
{
  server.handleClient();
#ifdef SUPPORT_MDNS
  MDNS.update();
#endif
#ifdef SUPPORT_SSDP
  SSDP_esp8266.handleClient();
#endif

  //lendo codigo?
  if (decoding_onoff)
  {
    //pisca led
    if ((millis() - old_millis) > LED_BLINK_INTERVAL)
    {
      old_millis = millis();
      led_status = (led_status == HIGH) ? LOW : HIGH;
      digitalWrite(LED_PIN, led_status);
    }
    //enable
    if (!irin_enable)
    {
      irrecv.enableIRIn(); // Start up the IR receiver.
      irin_enable = true;
      irin_timeout = false;
      ir_started = millis();
    }
    //check timeout
    if ((millis() - ir_started) > IR_TIMEOUT)
    {
      //timeout
      irin_timeout = true;
#ifdef DEBUG
      Serial.println("IR TIMEOUT");
#endif
      _end_ir();
    }

    //decode
    if (irrecv.decode(&irresult.results))
    {
#ifdef DEBUG
      dump_ir(irresult);
#endif
      //captura feita
      _end_ir();
    }
  }

//telegram loop
#ifdef SUPPORT_TELEGRAM
  bt_loop();
#endif
}

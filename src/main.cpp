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
bool irin_enable;
bool irin_timeout;

//
CODES irresult;

std::vector<CODES> ir_codes;

WiFiManager wm;
ESP8266WebServer server;
#ifdef SUPPORT_OTA
ESP8266HTTPUpdateServer httpUpdater;
#endif

bool decoding_onoff;

//led blink vars
#define LED_BLINK_INTERVAL 300
ESP8266Timer ITimer;

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

void IRAM_ATTR blink_led()
{
  static bool toggle = false;
  if (decoding_onoff)
  {
    digitalWrite(LED_PIN, toggle);
    toggle = !toggle;

    //timer1_write(600000);
    timer1_write(60000);
  }
}

void setup()
{

#ifdef SUPPORT_LITTLEFS
  LittleFS.begin();
#else
  SPIFFS.begin();
#endif

  WiFi.mode(WIFI_STA);

  _Serial.begin(115200);
  _Serial.println("");
  _Serial.println("WIFIIR - Starting");
  _Serial.printf("Build Version: %s %s\n", __DATE__, __TIME__);

#ifdef DEBUG
  _Serial.setDebugOutput(true);
  wm.setDebugOutput(true);
#else
  _Serial.setDebugOutput(false);
  wm.setDebugOutput(false);
#endif

#ifdef DEBUG_ESP
  dump_esp8266();
#endif

  if (!wm.autoConnect("WIFIIR"))
  {
#ifdef DEBUG
    _Serial.println("Failed to connect");
#endif
    ESP.restart();
    delay(1000);
  }
  else
  {
#ifdef DEBUG
    _Serial.println("Connected...");
#endif
  }

  //get internet time
  configTime(-3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

#ifdef SUPPORT_OTA
  httpUpdater.setup(&server, "/update");
#endif

  install_www_handlers();

  server.begin();

#ifdef DEBUG_ESP
  dump_fs();
#endif

  //discovery protocols
  wifiir_name_load();
  String s = "WIFIIR" + (wifiir_subname.isEmpty() ? "" : ("-" + wifiir_subname));
#ifdef DEBUG
  _Serial.printf("Discovery Protocols Name: %s\n", s.c_str());
#endif

#ifdef SUPPORT_MDNS
  if (MDNS.begin(s.c_str()))
  {
#ifdef DEBUG
    _Serial.println("MDNS OK");
#endif
  }
  else
  {
#ifdef DEBUG
    _Serial.println("MDNS NOK");
#endif
  }
  MDNS.addService("http", "tcp", 80);
#endif

#ifdef SUPPORT_NETBIOS
  if (NBNS.begin(s.c_str()))
  {
#ifdef DEBUG
    _Serial.println("NETBIOS OK");
#endif
  }
  else
  {
#ifdef DEBUG
    _Serial.println("NETBIOS NOK");
#endif
  }
#endif

#ifdef SUPPORT_LLMNR
  if (LLMNR.begin(s.c_str()))
  {
#ifdef DEBUG
    _Serial.println("LLMNR OK");
#endif
  }
  else
  {
#ifdef DEBUG
    _Serial.println("LLMNR NOK");
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
  _Serial.println("SSDP OK");
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

  //blinking LED setup
  irin_timeout = irin_enable = decoding_onoff = false;
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  ITimer.attachInterruptInterval(LED_BLINK_INTERVAL, blink_led);
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
  irin_enable = decoding_onoff = false;
  digitalWrite(LED_PIN, LOW);
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
    //habilita leitor
    if (!irin_enable)
    {
      irrecv.enableIRIn();
      irin_enable = true;
      irin_timeout = false;
    }

    //esperando resultado...
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

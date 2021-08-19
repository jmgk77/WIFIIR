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
bool decoding_onoff;
bool waiting_ir;
bool toggle;

//
CODES irresult;

std::vector<CODES> ir_codes;

WiFiManager wm;
ESP8266WebServer server;
#ifdef SUPPORT_OTA
ESP8266HTTPUpdateServer httpUpdater;
#endif

//led blink vars
#define LED_BLINK_INTERVAL 300
ESP8266Timer ITimer;

//device name
String wifiir_subname;

//
String boot_time;

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
  if (decoding_onoff)
  {
    digitalWrite(LED_PIN, toggle);
    toggle = !toggle;
  }
}

void setup()
{
  LittleFS.begin();

  WiFi.mode(WIFI_STA);

  _Serial.begin(115200);
  _Serial.println("");
  _Serial.println("WIFIIR - Starting");
  _Serial.printf("Build Version: %s (%s)\n", VERSION, BUILD_TIMESTAMP);
  wm.setDebugOutput(false);
#ifdef DEBUG
  _Serial.setDebugOutput(true);
#else
  _Serial.setDebugOutput(false);
#endif

#ifdef DEBUG_ESP
  dump_esp8266();
#endif

  //load device name
  wifiir_name_load();
  String device_name = "WIFIIR" + (wifiir_subname.isEmpty() ? "" : ("-" + wifiir_subname));
#ifdef DEBUG
  _Serial.printf("Device Name: %s\n", device_name.c_str());
#endif

  //set name in router
  WiFi.hostname(device_name);

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
  configTime("<-03>3", "pool.ntp.org");
  //verifica 2021...
  while (time(nullptr) < 1609459200)
  {
#ifdef DEBUG
    _Serial.print(".");
#endif
    delay(100);
  }
#ifdef DEBUG
  _Serial.println();
#endif
  time_t tnow = time(nullptr);
  boot_time = String(ctime(&tnow));
#ifdef DEBUG
  _Serial.println(boot_time);
#endif

#ifdef SUPPORT_OTA
  httpUpdater.setup(&server, "/update");
#endif

  install_www_handlers();

  server.begin();

#ifdef DEBUG_ESP
  dump_fs();
#endif

  //discovery protocols
#ifdef SUPPORT_MDNS
  if (MDNS.begin(device_name))
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
  if (NBNS.begin(device_name.c_str()))
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
  if (LLMNR.begin(device_name.c_str()))
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
  SSDP_esp8266.setName(device_name);
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
  irin_enable = decoding_onoff = false;
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  ITimer.attachInterruptInterval(LED_BLINK_INTERVAL * 1000, blink_led);
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
      waiting_ir = true;
    }

    //esperando resultado...
    if (irrecv.decode(&irresult.results))
    {
#ifdef DEBUG
      dump_ir(irresult);
#endif
      //captura feita
      waiting_ir = false;
      _end_ir();
    }
  }

//telegram loop
#ifdef SUPPORT_TELEGRAM
  bt_loop();
#endif
}

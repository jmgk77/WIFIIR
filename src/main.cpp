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

#ifdef ESP32
#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>

// From v1.1.1
#include <WiFiMulti.h>
WiFiMulti wifiMulti;

// LittleFS has higher priority than SPIFFS
#define USE_LITTLEFS true
#define USE_SPIFFS false

#if USE_LITTLEFS
// Use LittleFS
#include "FS.h"

// The library will be depreciated after being merged to future major Arduino esp32 core release 2.x
// At that time, just remove this library inclusion
#include <LITTLEFS.h> // https://github.com/lorol/LITTLEFS

FS *filesystem = &LITTLEFS;
#define FileFS LITTLEFS
#define FS_Name "LittleFS"
#elif USE_SPIFFS
#include <SPIFFS.h>
FS *filesystem = &SPIFFS;
#define FileFS SPIFFS
#define FS_Name "SPIFFS"
#else
// +Use FFat
#include <FFat.h>
FS *filesystem = &FFat;
#define FileFS FFat
#define FS_Name "FFat"
#endif
//////

#define ESP_getChipId() ((uint32_t)ESP.getEfuseMac())

#define LED_BUILTIN 2
#define LED_ON HIGH
#define LED_OFF LOW

#else

#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>

// From v1.1.1
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

#define USE_LITTLEFS true

#if USE_LITTLEFS
#include <LittleFS.h>
FS *filesystem = &LittleFS;
#define FileFS LittleFS
#define FS_Name "LittleFS"
#else
FS *filesystem = &SPIFFS;
#define FileFS SPIFFS
#define FS_Name "SPIFFS"
#endif
//////

#define ESP_getChipId() (ESP.getChipId())

#define LED_ON LOW
#define LED_OFF HIGH
#endif

// SSID and PW for Config Portal
String ssid = "ESP_" + String(ESP_getChipId(), HEX);
const char *password = "your_password";

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

// From v1.1.0
// You only need to format the filesystem once
//#define FORMAT_FILESYSTEM       true
#define FORMAT_FILESYSTEM false

#define MIN_AP_PASSWORD_SIZE 8

#define SSID_MAX_LEN 32
//From v1.0.10, WPA2 passwords can be up to 63 characters long.
#define PASS_MAX_LEN 64

typedef struct
{
  char wifi_ssid[SSID_MAX_LEN];
  char wifi_pw[PASS_MAX_LEN];
} WiFi_Credentials;

typedef struct
{
  String wifi_ssid;
  String wifi_pw;
} WiFi_Credentials_String;

#define NUM_WIFI_CREDENTIALS 2

typedef struct
{
  WiFi_Credentials WiFi_Creds[NUM_WIFI_CREDENTIALS];
} WM_Config;

WM_Config WM_config;

#define CONFIG_FILENAME F("/wifi_cred.dat")
//////

#include <ESPAsync_WiFiManager.h> //https://github.com/khoih-prog/ESPAsync_WiFiManager

#define HTTP_PORT 80

#define USE_ESP_WIFIMANAGER_NTP false
#define _ESPASYNC_WIFIMGR_LOGLEVEL_ 3

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

AsyncWebServer server(80);

#ifdef SUPPORT_OTA
ESP8266HTTPUpdateServer httpUpdater;
#endif

bool decoding_onoff;
bool toggle;

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
  Serial.begin(115200);
  LittleFS.begin();

  WiFi.mode(WIFI_STA);

  _Serial.println("");
  _Serial.println("WIFIIR - Starting");
  _Serial.printf("Build Version: %s %s\n", WIFIIR_VERSION);

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

  DNSServer dnsServer;
  ESPAsync_WiFiManager ESPAsync_wifiManager(&server, &dnsServer, "WIFIIR");

#ifdef DEBUG
  ESPAsync_wifiManager.setDebugOutput(true);
#else
  ESPAsync_wifiManager.setDebugOutput(false);
#endif

  if (!ESPAsync_wifiManager.startConfigPortal())
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

  //server.begin();

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
  irin_timeout = irin_enable = decoding_onoff = false;
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
  //server.handleClient();
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

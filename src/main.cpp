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

const uint16_t kIrLedPin = D2;
IRsend irsend(kIrLedPin);

//irRecv vars
const uint16_t kRecvPin = D7;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);
bool irin_enable = false;

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
#define LED_BLINK_INTERVAL 500
#define LED_PIN D5
unsigned long old_millis = 0;
int led_status = LOW;

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

#ifdef DEBUG
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

  MDNS.begin("WIFIIR");
#ifdef SUPPORT_OTA
  httpUpdater.setup(&server, "/update");
#endif

  install_www_handlers();

  server.begin();
  MDNS.addService("http", "tcp", 80);

  //blinking LED setup
  decoding_onoff = false;
  pinMode(LED_PIN, OUTPUT);
  led_status = LOW;
  digitalWrite(LED_PIN, led_status);

  //carrega botões salvos
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

#ifdef DEBUG
  dump_fs();
#endif

  codes_load();

  //setup telegram bot
#ifdef SUPPORT_TELEGRAM
  telegram_load();
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

void loop()
{
  server.handleClient();
  MDNS.update();

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
    }
    //decode
    if (irrecv.decode(&irresult.results))
    {
#ifdef DEBUG
      dump_ir(irresult);
#endif
      //captura feita
      irrecv.disableIRIn();
      led_status = LOW;
      digitalWrite(LED_PIN, led_status);
      irin_enable = false;
      decoding_onoff = false;
    }
  }

//telegram loop
#ifdef SUPPORT_TELEGRAM
  bt_loop();
#endif
}

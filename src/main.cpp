#include <Arduino.h>

#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

WiFiManager wm;
ESP8266WebServer server;
ESP8266HTTPUpdateServer httpUpdater;

#define DEBUG 1

/*
██╗   ██╗████████╗██╗██╗
██║   ██║╚══██╔══╝██║██║
██║   ██║   ██║   ██║██║
██║   ██║   ██║   ██║██║
╚██████╔╝   ██║   ██║███████╗
 ╚═════╝    ╚═╝   ╚═╝╚══════╝
*/

void info()
{
#ifdef DEBUG
  Serial.println("ESP8266 INFO");
  Serial.printf("\tESP.getResetReason()\t%s\n", ESP.getResetReason().c_str());
  Serial.printf("\tESP.getFreeHeap()\t%d\n", ESP.getFreeHeap());
  Serial.printf("\tESP.getHeapFragmentation()\t%d\n", ESP.getHeapFragmentation());
  Serial.printf("\tESP.getMaxFreeBlockSize()\t%d\n", ESP.getMaxFreeBlockSize());
  Serial.printf("\tESP.getChipId()\t%d\n", ESP.getChipId());
  Serial.printf("\tESP.getCoreVersion()\t%s\n", ESP.getCoreVersion().c_str());
  Serial.printf("\tESP.getSdkVersion()\t%s\n", ESP.getSdkVersion());
  Serial.printf("\tESP.getCpuFreqMHz()\t%d\n", ESP.getCpuFreqMHz());
  Serial.printf("\tESP.getSketchSize()\t%d\n", ESP.getSketchSize());
  Serial.printf("\tESP.getFreeSketchSpace()\t%d\n", ESP.getFreeSketchSpace());
  Serial.printf("\tESP.getSketchMD5()\t%s\n", ESP.getSketchMD5().c_str());
  Serial.printf("\tESP.getFlashChipId()\t%d\n", ESP.getFlashChipId());
  Serial.printf("\tESP.getFlashChipSize()\t%d\n", ESP.getFlashChipSize());
  Serial.printf("\tESP.getFlashChipRealSize()\t%d\n", ESP.getFlashChipRealSize());
  Serial.printf("\tESP.getFlashChipSpeed()\t%u\n", (unsigned int)ESP.getFlashChipSpeed);
  Serial.printf("\tESP.getCycleCount()\t%d\n", ESP.getCycleCount());
  Serial.printf("\tESP.random()\t%d\n", ESP.random());
  Serial.printf("\tESP.checkFlashCRC()\t%s\n", ESP.checkFlashCRC() ? "OK" : "NOK");
  Serial.printf("\tESP.getVcc()\t%d\n", ESP.getVcc());
#endif
}

/*
██╗    ██╗██╗    ██╗██╗    ██╗    ██╗  ██╗ █████╗ ███╗   ██╗██████╗ ██╗     ███████╗██████╗ ███████╗
██║    ██║██║    ██║██║    ██║    ██║  ██║██╔══██╗████╗  ██║██╔══██╗██║     ██╔════╝██╔══██╗██╔════╝
██║ █╗ ██║██║ █╗ ██║██║ █╗ ██║    ███████║███████║██╔██╗ ██║██║  ██║██║     █████╗  ██████╔╝███████╗
██║███╗██║██║███╗██║██║███╗██║    ██╔══██║██╔══██║██║╚██╗██║██║  ██║██║     ██╔══╝  ██╔══██╗╚════██║
╚███╔███╔╝╚███╔███╔╝╚███╔███╔╝    ██║  ██║██║  ██║██║ ╚████║██████╔╝███████╗███████╗██║  ██║███████║
 ╚══╝╚══╝  ╚══╝╚══╝  ╚══╝╚══╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝
*/

void handle_reset()
{
#ifdef DEBUG
  Serial.println("Reseting...");
#endif
  server.send(200, "text/plain", "Reseting...");
  wm.resetSettings();
  ESP.restart();
}

void handle_root()
{
#ifdef DEBUG
  Serial.println("Main...");
#endif
  server.send(200, "text/plain", "Main...");
}

void handle_404()
{
#ifdef DEBUG
  Serial.println("Not found...");
#endif
  server.send(200, "text/plain", "404...");
}

void install_www_handlers()
{
  server.on("/reset", handle_reset);
  server.on("/", handle_root);
  server.onNotFound(handle_404);
}

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

  info();

#ifdef DEBUG
  Serial.setDebugOutput(true);
  wm.setDebugOutput(true);
#else
  Serial.setDebugOutput(false);
  wm.setDebugOutput(false);
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
  httpUpdater.setup(&server);

  install_www_handlers();

  server.begin();
  MDNS.addService("http", "tcp", 80);
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
}

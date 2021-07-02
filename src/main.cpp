#include <Arduino.h>

#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

WiFiManager wm;
ESP8266WebServer server;
ESP8266HTTPUpdateServer httpUpdater;

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
  Serial.setDebugOutput(true);
  delay(3000);

  if (!wm.autoConnect("WIFIIR"))
  {
    Serial.println("Failed to connect");
    ESP.restart();
    delay(1000);
  }
  else
  {
    Serial.println("Connected...");
  }

  MDNS.begin("WIFIIR");
  httpUpdater.setup(&server);

  server.on("/reset", []()
            {
              Serial.println("Reseting...");
              server.send(200, "text/plain", "Reseting...");
              wm.resetSettings();
              ESP.restart();
            });
  server.on("/", []()
            {
              Serial.println("Main...");
              server.send(200, "text/plain", "Main...");
            });

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

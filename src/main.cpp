//WIFIIR - Controlador IR via internet
//
//(c) JMGK 2021

#define DEBUG 1

#include <Arduino.h>

#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

WiFiManager wm;
ESP8266WebServer server;
ESP8266HTTPUpdateServer httpUpdater;

bool decoding_onoff;

//led blink vars
#define LED_BLINK_INTERVAL 500
#define LED_PIN D5
unsigned long old_millis = 0;
int led_status = LOW;

/*
██╗    ██╗██╗    ██╗██╗    ██╗    ██╗  ██╗ █████╗ ███╗   ██╗██████╗ ██╗     ███████╗██████╗ ███████╗
██║    ██║██║    ██║██║    ██║    ██║  ██║██╔══██╗████╗  ██║██╔══██╗██║     ██╔════╝██╔══██╗██╔════╝
██║ █╗ ██║██║ █╗ ██║██║ █╗ ██║    ███████║███████║██╔██╗ ██║██║  ██║██║     █████╗  ██████╔╝███████╗
██║███╗██║██║███╗██║██║███╗██║    ██╔══██║██╔══██║██║╚██╗██║██║  ██║██║     ██╔══╝  ██╔══██╗╚════██║
╚███╔███╔╝╚███╔███╔╝╚███╔███╔╝    ██║  ██║██║  ██║██║ ╚████║██████╔╝███████╗███████╗██║  ██║███████║
 ╚══╝╚══╝  ╚══╝╚══╝  ╚══╝╚══╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝
*/

const char *html_header = "\
<!DOCTYPE html>\
<html>\
<head>\
<title>WIFIIR</title>\
<meta charset='UTF-8'/>\
<link rel='shortcut icon' href='data:image/x-icon;base64,iVBORw0KGgoAAAANSUhEUgAAABoAAAAaCAYAAACpSkzOAAAABmJLR0QA/wD/AP+gvaeTAAABoklEQVRIie3UvWsUURQF8F+2VEyKsJgIiokRApZa+VGIKVRSmVJtAgr6B1gEO/VvsLDWNkbUCKKFhbZpwoYIClZ20WiXj7WYu8zsM7M7E6JVDlyYefecc997986wj11ioE9+FNO4gFM4hqHI/cQ3LOMDXuJ73Q1cwiI20a4Ym3iFi1UKnMCbGuZlsRheO+Iq1hLBFj5iTnZ9EzgYcTLW5vApuEXtOq6nRe4lxC087bWrHTCBZ4nPNu53COOx0El+wemCwRHclV1HC78jWniNO8Hp4Ex4FIsdh0HZtLTxFsMhGMFjbOjfk43gHg7tcHi1w/tQZxdNnEcj3q/gR4UCaazhcng0wrOpBLP+Huv3uIVJ+TBM4nbk0jGfLTMv4rPufk1V0Ezha0G3WqXQwyC/kx97FI+whF8RS8EdCU5TfroHVQrBgcLzzTAu68s6bpRoK2NG9+iXxTau9TJq9ErinPzH2wqzoYgZrERuAGdrHqILR/ECT2TfW4rByC0Ed88wHqbPMbaXxikW5H2ZryPs16MU7Zr8XWNMdpJ5//jq9vH/8QdSN6mUF/XpPQAAAABJRU5ErkJggg==' />\
<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/mini.css/3.0.1/mini-default.min.css'>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
</head>\
<body>\
<header class='sticky'>\
<div class='card warning' style='width: 80%;margin: 0 auto;'>\
<div class='row'>\
<p style='width: 80%;margin: 0 auto;'>WIFIIR - Controle remoto IR via WIFI</p>\
</div>\
</div>\
</header>";

const char *html_footer = "\
<footer class='sticky'>\
<label for='drawer-control' class='drawer-toggle'></label>\
<input type='checkbox' id='drawer-control' class='drawer'>\
<nav>\
<label for='drawer-control' class='drawer-close'></label>\
<a class='doc' href='/controle'>Controle</a><br>\
<a class='doc' href='/add'>Adicionar</a><br>\
<a class='doc' href='/reset'>Resetar</a><br>\
<a class='doc' href='/update'>Update firmware</a><br>\
</nav>\
</footer>\
</body>\
</html>";

void send_html(const char *h)
{
  char *r = (char *)malloc(2048);
  strcpy(r, html_header);
  strcat(r, h);
  strcat(r, html_footer);
  server.send(200, "text/html", r);
  free(r);
}

void handle_reset()
{
  send_html("<input type='button' value='Factory Reset' onclick='location.href=\"/reset2\"'>");
}

void handle_reset2()
{
  send_html("<p>Reseting...</p>");
  wm.resetSettings();
  delay(1000);
  wm.reboot();
  delay(2000);
}

void handle_root()
{
  //###list codes
  send_html("<p>Main...</p>");
}

void handle_add()
{
  String button;
  button = "";
  if (server.hasArg("button"))
  {
    button = server.arg("button");
  }
  char *r = (char *)malloc(2048);
  //###warning if 'button' exists
  strcpy(r, "<br><br><br>\
<form action='/save' method='get'>\
<p>1. Entre o nome do botão que você quer adicionar</p>\
<p>2. Espere a luz do WIFIIR começar a piscar</p>\
<p>3. Aperte o botão no controle até a luz desligar</p>\
<p>4. Clique em ADICIONAR abaixo</p>\
<input type='text' name='button' value='");
  strcat(r, button.c_str());
  strcat(r, "'>\
<input type='submit' value='Submit'>\
<br><br><br>\
<\form>");
  send_html(r);
  free(r);
  //inicia leitor de IR
  decoding_onoff = true;
}

void handle_save()
{
  String button;
  button = "";
  if (server.hasArg("button"))
  {
    button = server.arg("button");
  }
  if ((decoding_onoff) || (button.isEmpty()))
  {
#ifdef DEBUG
    Serial.print("SAVE::redirecting back to ADD");
    if (decoding_onoff)
    {
      Serial.print("::without code");
    }
    if (button.isEmpty())
    {
      Serial.print("::empty button name");
    }
    Serial.println();
#endif
    //redirect to /add with "button" as param
    char *r = (char *)malloc(2048);
    strcpy(r, "<script>document.location.href = '/add");
    if (!button.isEmpty())
    {
      strcat(r, "?button=");
      strcat(r, button.c_str());
    }
    strcat(r, "'</script>");
    send_html(r);
    free(r);
  }
  else
  {
    //###save code->button
  }
}

void handle_update()
{
  send_html("<br><br><br>\
<form method='POST' action='/update2' enctype='multipart/form-data'>\
Firmware:<br>\
<input type='file' accept='.bin,.bin.gz' name='firmware'>\
<input type='submit' value='Update Firmware'>\
</form>\
<br><br><br>");
}

void handle_404()
{
  send_html("<p>Not found!</p>");
}

void install_www_handlers()
{
  server.on("/", handle_root);
  server.on("/controle", handle_root);
  server.on("/add", handle_add);
  server.on("/save", handle_save);
  server.on("/reset", handle_reset);
  server.on("/reset2", handle_reset2);
  server.on("/update", handle_update);
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
  httpUpdater.setup(&server, "/update2");

  install_www_handlers();

  server.begin();
  MDNS.addService("http", "tcp", 80);

  //blinking LED setup
  decoding_onoff = false;
  pinMode(LED_PIN, OUTPUT);
  led_status = LOW;
  digitalWrite(LED_PIN, led_status);
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
    //decode
    //###decode
  }
}

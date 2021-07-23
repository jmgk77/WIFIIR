/*
██╗    ██╗██╗    ██╗██╗    ██╗    ██╗  ██╗ █████╗ ███╗   ██╗██████╗ ██╗     ███████╗██████╗ ███████╗
██║    ██║██║    ██║██║    ██║    ██║  ██║██╔══██╗████╗  ██║██╔══██╗██║     ██╔════╝██╔══██╗██╔════╝
██║ █╗ ██║██║ █╗ ██║██║ █╗ ██║    ███████║███████║██╔██╗ ██║██║  ██║██║     █████╗  ██████╔╝███████╗
██║███╗██║██║███╗██║██║███╗██║    ██╔══██║██╔══██║██║╚██╗██║██║  ██║██║     ██╔══╝  ██╔══██╗╚════██║
╚███╔███╔╝╚███╔███╔╝╚███╔███╔╝    ██║  ██║██║  ██║██║ ╚████║██████╔╝███████╗███████╗██║  ██║███████║
 ╚══╝╚══╝  ╚══╝╚══╝  ╚══╝╚══╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝
*/

#include "www.h"

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
<a class='doc' href='/config'>Configurações</a><br>\
</nav>\
</footer>\
</body>\
</html>";

const char *html_css = "\
<style>\
.wifi, .wifi:focus, .wifi:hover {\
background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAAmJLR0QA/4ePzL8AAAAHdElNRQflBxMQGDNbPF7PAAAA7klEQVQoz5XRPUuCAQDE8d+jUgRGWImbNFj2EEEIDS1NCdHiEkV9nfCrFEWLSwQ1tTi0BCFPL0u0iWEEQihqTVKUFd12x3+5O/5Q8MUl0PX2HUhaEJo2go4nkZrWABi1ZlnDtUctJGUtSrt0rh3I2vGsoi5txiSaHjRklKQcBPacuRBal1TXxKSMllORVcXAlBeb5py40jOGV3FLNtw5NhFg1oojI4ry4ui5daZjS9V9gJi+ebtqqhpIW7Fg342Y/qBuQR7k5EBeYdhQ20JEDoevmlI2Lqks9REmPgFtXSG62j99ESqhIvrtvMD/9A6R0TwXv22H+QAAACV0RVh0ZGF0ZTpjcmVhdGUAMjAyMS0wNy0xOVQxNjoyNDoyNSswMDowMHcnQy4AAAAldEVYdGRhdGU6bW9kaWZ5ADIwMjEtMDctMTlUMTY6MjQ6MjUrMDA6MDAGevuSAAAAAElFTkSuQmCC');\
background-repeat: no-repeat;\
background-color: #f8f8f8;\
}\
.trash, .trash:focus, .trash:hover {\
background-image: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAAmJLR0QA/4ePzL8AAAAHdElNRQflBxMQGDNbPF7PAAAA40lEQVQoz43RvU4CYRCF4Wd1WaJBIo00NhSa0EinlfRejom34S14F8TEzlKjrSbGgoLEn8TChg27LJ+FwLKGwlNMMfPOmclMZKHIuYaxSNuVpzLNoWOZHadu5GaObLkTq7n3EiN14trI87zpDew6c0uMkVcPPlW158CIDRDUtfTR1UVfS13gFwgKsaYeOjroaYoVwsIhVxMEVmJNvnBgKvFXiWkJ5GuBvAqESjlUgWytQ/avETGYSOZnj5YPSExKILdtbMj8nkNjibQEUvu+DPAIBmYa3i0t2y58K1Y22NR06YMfisxC2gggFqAAAAAldEVYdGRhdGU6Y3JlYXRlADIwMjEtMDctMTlUMTY6MjQ6MjUrMDA6MDB3J0MuAAAAJXRFWHRkYXRlOm1vZGlmeQAyMDIxLTA3LTE5VDE2OjI0OjI1KzAwOjAwBnr7kgAAAABJRU5ErkJggg==');\
background-repeat: no-repeat;\
background-color: #f8f8f8;\
}\
</style>";

void send_html(const char *h)
{
  char *r = (char *)_malloc(strlen(h) + strlen(html_header) + strlen(html_footer) + 16);
  strcpy(r, html_header);
  strcat(r, h);
  strcat(r, html_footer);
  server.send(200, "text/html", r);
#ifdef DEBUG_SEND
  Serial.println(r);
#endif
  free(r);
}

void handle_root()
{
  char *r = (char *)_malloc(strlen(html_css) + 81 + (ir_codes.size() * 301) + 16);
  strcpy(r, html_css);
  strcat(r, "<table><tbody><thead><tr><th>Name</th><th>Button</th><th>Delete</th></tr></thead>");
  char n[8];
  int c = 0;
  for (auto i = ir_codes.cbegin(); i != ir_codes.cend(); ++i)
  {
    itoa(c, n, 10);
    strcat(r, "<tr><td data-label='Name'>");
    //name
    strcat(r, (*i).name);
    strcat(r, "</td><td data-label='Button'><input class='wifi' type='button' onclick='window.location.href=\"/press?button=");
    strcat(r, n);
    strcat(r, "\";'/></td><td data-label='Delete'><input type='button' class='trash' onclick='window.location.href=\"/del?button=");
    strcat(r, n);
    strcat(r, "\";'/></td></tr>");
    c++;
  }
  strcat(r, "</tbody></table>");
  send_html(r);
  free(r);
}

void handle_add()
{
  String button;
  button = "";
  if (server.hasArg("button"))
  {
    button = server.arg("button");
  }
  char *r = (char *)_malloc(512);
  r[0] = 0;
  if (server.hasArg("empty"))
  {
    strcat(r, "<div class='card error'>Preencha o nome do botão</div>");
  }
  if (server.hasArg("nocode"))
  {
    strcat(r, "<div class='card error'>Erro na leitura do botão</div>");
  }
  strcat(r, "<form action='/save' method='get'>\
<p>1. Entre o nome do botão que você quer adicionar</p>\
<p>2. Espere a luz do WIFIIR começar a piscar</p>\
<p>3. Aperte o botão no controle até a luz desligar</p>\
<p>4. Clique em ADICIONAR abaixo</p>\
<input type='text' name='button' maxlength='31' value='");
  strcat(r, button.c_str());
  strcat(r, "'>\
<input type='submit' value='Salvar'>\
</form>");
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
    //redirect to /add with "button" as param
    char *r = (char *)_malloc(512);
    strcpy(r, "<script>document.location.href = '/add");
    if (!button.isEmpty())
    {
      strcat(r, "?button=");
      strcat(r, button.c_str());
    }
    else
    {
      strcat(r, "?empty=1");
    }
    if (decoding_onoff)
    {
      strcat(r, "&nocode=1");
    }
    strcat(r, "'</script>");
    send_html(r);
    free(r);
  }
  else
  {
    //save button info
    strncpy(irresult.name, button.c_str(), sizeof(irresult.name) - 1);
    ir_codes.push_back(irresult);
    codes_save();
    send_html("<div class='card warning'>Botão Salvo!</div><script>setTimeout(function (){document.location.href = '/controle';}, 500);</script>");
  }
}

void handle_press()
{
  if (server.hasArg("button"))
  {
    int button = atoi(server.arg("button").c_str());
    ir_send(button);
  }
  send_html("<div class='card warning'>Botão enviado!</div><script>setTimeout(function (){document.location.href = '/controle';}, 500);</script>");
}

void handle_del()
{
  int button;
  if (server.hasArg("button"))
  {
    button = atoi(server.arg("button").c_str());
    ir_codes.erase(ir_codes.begin() + button);
    codes_save();
  }
  send_html("<div class='card warning'>Botão deletado!</div><script>setTimeout(function (){document.location.href = '/controle';}, 500);</script>");
}

void handle_config()
{
  char *r = (char *)_malloc(1024);
  strcpy(r, "<form action='/clear' method='get'>\
Apagar botões salvos:<input type='submit' value='Apagar'>\
</form>\
<form action='/reboot' method='get'>\
Reiniciar WIFIIR:<input type='submit' value='Reiniciar'>\
</form>\
<form action='/reset' method='get'>\
Limpar configurações WIFI:<input type='submit' value='Limpar'>\
</form>");
#ifdef SUPPORT_TELEGRAM
  strcpy(r, "<form action='/token' method='get'>\
Telegram Token:<input type='checkbox' onchange='document.getElementById(\"token\").disabled=!this.checked;\
document.getElementById(\"btoken\").disabled=!this.checked;'>\
<input type='text' id='token' name='token' disabled value='");
  strcat(r, bt_token.c_str());
  strcat(r, "'/><input type='submit' id='btoken' disabled value='Salvar'></form>");
#endif
#ifdef SUPPORT_OTA
  strcat(r, "<form method='POST' action='/update' enctype='multipart/form-data'>\
Atualizar Firmware:<input type='file' accept='.bin,.bin.gz' name='firmware'>\
<input type='submit' value='Update Firmware'>\
</form>");
#endif
  strcat(r, "<br>Build Version: ");
  strcat(r, __DATE__);
  strcat(r, " ");
  strcat(r, __TIME__);
  strcat(r, "<br><br>");
  send_html(r);
  free(r);
}

#ifdef SUPPORT_TELEGRAM
void handle_token()
{
  if (server.hasArg("token"))
  {
    bt_token = server.arg("token");
    bt_setup();
    telegram_save();
  }
  send_html("<div class='card warning'>Token Salvo!</div><script>setTimeout(function (){document.location.href = '/controle';}, 500);</script>");
}
#endif

void handle_clear()
{
  ir_codes.clear();
#ifdef SUPPORT_LITTLEFS
  LittleFS.remove("/codes.bin");
#else
  SPIFFS.remove("/codes.bin");
#endif
  send_html("<div class='card warning'>Botões limpos</div><script>setTimeout(function (){document.location.href = '/controle';}, 500);</script>");
}

void reboot()
{
  delay(1000);
  wm.reboot();
  delay(2000);
}

void handle_reset()
{
  send_html("<p>Reseting...</p>");
  wm.resetSettings();
  reboot();
}

void handle_reboot()
{
  send_html("<p>Rebooting...</p>");
  reboot();
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
  server.on("/press", handle_press);
  server.on("/del", handle_del);
#ifdef SUPPORT_TELEGRAM
  server.on("/token", handle_token);
#endif
  server.on("/save", handle_save);
  server.on("/reboot", handle_reboot);
  server.on("/reset", handle_reset);
  server.on("/config", handle_config);
  server.on("/clear", handle_clear);
  server.onNotFound(handle_404);
}

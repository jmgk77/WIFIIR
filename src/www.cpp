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
<!DOCTYPE html><html><head><title>WIFIIR</title>\
<meta charset='UTF-8'/>\
<link rel='shortcut icon' href='data:image/x-icon;base64,iVBORw0KGgoAAAANSUhEUgAAABoAAAAaCAYAAACpSkzOAAAABmJLR0QA/wD/AP+gvaeTAAABoklEQVRIie3UvWsUURQF8F+2VEyKsJgIiokRApZa+VGIKVRSmVJtAgr6B1gEO/VvsLDWNkbUCKKFhbZpwoYIClZ20WiXj7WYu8zsM7M7E6JVDlyYefecc997986wj11ioE9+FNO4gFM4hqHI/cQ3LOMDXuJ73Q1cwiI20a4Ym3iFi1UKnMCbGuZlsRheO+Iq1hLBFj5iTnZ9EzgYcTLW5vApuEXtOq6nRe4lxC087bWrHTCBZ4nPNu53COOx0El+wemCwRHclV1HC78jWniNO8Hp4Ex4FIsdh0HZtLTxFsMhGMFjbOjfk43gHg7tcHi1w/tQZxdNnEcj3q/gR4UCaazhcng0wrOpBLP+Huv3uIVJ+TBM4nbk0jGfLTMv4rPufk1V0Ezha0G3WqXQwyC/kx97FI+whF8RS8EdCU5TfroHVQrBgcLzzTAu68s6bpRoK2NG9+iXxTau9TJq9ErinPzH2wqzoYgZrERuAGdrHqILR/ECT2TfW4rByC0Ed88wHqbPMbaXxikW5H2ZryPs16MU7Zr8XWNMdpJ5//jq9vH/8QdSN6mUF/XpPQAAAABJRU5ErkJggg=='/>\
<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/mini.css/3.0.1/mini-default.min.css'>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
</head><body><header class='sticky'>\
<div class='card warning' style='width: 80%;margin: 0 auto;'>\
<div class='row'><p style='width: 80%;margin: 0 auto;'>WIFIIR - Controle remoto IR via WIFI</p>\
</div></div></header>";

const char *html_footer = "\
<footer class='sticky'><label for='drawer-control' class='drawer-toggle'></label>\
<input type='checkbox' id='drawer-control' class='drawer'>\
<nav><label for='drawer-control' class='drawer-close'></label>\
<a class='doc' href='/controle'>Controle</a><br>\
<a class='doc' href='/add'>Adicionar</a><br>\
<a class='doc' href='/config'>Configurações</a><br>\
</nav></footer></body></html>";

const char *html_css = "\
<style>\
.wifi,.wifi:focus,.wifi:hover{\
background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAAmJLR0QA/4ePzL8AAAAHdElNRQflBxMQGDNbPF7PAAAA7klEQVQoz5XRPUuCAQDE8d+jUgRGWImbNFj2EEEIDS1NCdHiEkV9nfCrFEWLSwQ1tTi0BCFPL0u0iWEEQihqTVKUFd12x3+5O/5Q8MUl0PX2HUhaEJo2go4nkZrWABi1ZlnDtUctJGUtSrt0rh3I2vGsoi5txiSaHjRklKQcBPacuRBal1TXxKSMllORVcXAlBeb5py40jOGV3FLNtw5NhFg1oojI4ry4ui5daZjS9V9gJi+ebtqqhpIW7Fg342Y/qBuQR7k5EBeYdhQ20JEDoevmlI2Lqks9REmPgFtXSG62j99ESqhIvrtvMD/9A6R0TwXv22H+QAAACV0RVh0ZGF0ZTpjcmVhdGUAMjAyMS0wNy0xOVQxNjoyNDoyNSswMDowMHcnQy4AAAAldEVYdGRhdGU6bW9kaWZ5ADIwMjEtMDctMTlUMTY6MjQ6MjUrMDA6MDAGevuSAAAAAElFTkSuQmCC);\
background-repeat:no-repeat;\
background-color:#f8f8f8}\
.edit,.edit:focus,.edit:hover{\
background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAAYQAAAGEBVSuQvQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAEKSURBVDiNldO9SgNBFAXgL0uw8AcUC5uAiKmshICdYOMjiNhaSCIWgr0voIW9T6CgjW1A8Aks7URQtBBBhARE1GImsI4TFw8sd4d775mzezg1AStYRM9PLOMCp4agHusCjjL9Ao9Yx0mOoIj1a8gFPVzjHht/KRiGc2zjFUt4x1lusFNBNEC7SkEDM5nFW7zkGFOCcUxl5p5iraWNIjk30MIz5nCJLh5yt+cUdOND+Ps5zOJN/KRUQRXmsSU4M/1fgqZg5x2usFNuVtnYxG58H8MxRssDbXn7csv75eWBLXWsYSKeR+JwH5tCqD6xh0Ol0P3yNUELk0JaP3AgSWxVFlajihshF/104BuZGSzACO/S3wAAAABJRU5ErkJggg==);\
background-repeat:no-repeat;\
background-color:#f8f8f8}\
.up,.up:focus,.up:hover{\
background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAAdAAAAHQBMYXlgQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAADwSURBVDiNldHBCoIwGMDxdfMuRI6iUua8FOaQmlB9YvRkklKv1K0X6BxCQXTuEdbJKN2mDnYZ3//H2HpCCKRa/nrnIoTQ9XK+KYeEENLtR9vQxMO3iYdvP9qGqjltPOdxMudxokO0cXmmQxrjJqRVrENaxypEGQOAgW1SYJsUAGCoEDQYO09ZbE3JnQZRSoMotabkLkP6o/GrdsUy9hg/lGce4wcZUvsFADCsifPwGM+qgx7jmTVxHlWkFrtslase0GWrvIr8xTTgx6ZfoAE//iKdYhmCsE0KypantvEXYcsTtkmBZpv9pmv8fZNFmHwADqQjycOvhksAAAAASUVORK5CYII=);\
background-repeat:no-repeat;\
background-color:#f8f8f8}\
.down,.down:focus,.down:hover{\
background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAAdAAAAHQBMYXlgQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAEhSURBVDiNnZO/TsMwEIevTJmQUAUiSZX+oWnaISFKFYk4Q2zyGFl4GD8IC48BEQvMiKFCVVVVqEszWMrEdkxFbWKHgCVPvu/z6Xc2+CQJERH+s72Y3oA5miydgPC/wk5AuDmaLE9sy3BLUWTTecyh5ZrOY16KIrMtwwVEBEqppg/tdZtOnIBwfWivKaUaIsLPQRtJFT4S/CaRwTWBSqKCERE6l/2rzwvDunt7eXrch8QY0z4228Xp2fkDAEApiszpm7M8z7/2NdfkNt1tN/fgkyTsGj3hRSytdTIYr/TBeFW92YtY2jV6widJCIiolEgfzwF8lEEbSRWuhdgkkcHSKcgkKlgqqEqaYKXgUNIEIyJ0EFH5adwomQEAvL8+L1Q130T1AeyZwbdgAAAAAElFTkSuQmCC);\
background-repeat:no-repeat;\
background-color:#f8f8f8}\
.trash,.trash:focus,.trash:hover{\
background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAAmJLR0QA/4ePzL8AAAAHdElNRQflBxMQGDNbPF7PAAAA40lEQVQoz43RvU4CYRCF4Wd1WaJBIo00NhSa0EinlfRejom34S14F8TEzlKjrSbGgoLEn8TChg27LJ+FwLKGwlNMMfPOmclMZKHIuYaxSNuVpzLNoWOZHadu5GaObLkTq7n3EiN14trI87zpDew6c0uMkVcPPlW158CIDRDUtfTR1UVfS13gFwgKsaYeOjroaYoVwsIhVxMEVmJNvnBgKvFXiWkJ5GuBvAqESjlUgWytQ/avETGYSOZnj5YPSExKILdtbMj8nkNjibQEUvu+DPAIBmYa3i0t2y58K1Y22NR06YMfisxC2gggFqAAAAAldEVYdGRhdGU6Y3JlYXRlADIwMjEtMDctMTlUMTY6MjQ6MjUrMDA6MDB3J0MuAAAAJXRFWHRkYXRlOm1vZGlmeQAyMDIxLTA3LTE5VDE2OjI0OjI1KzAwOjAwBnr7kgAAAABJRU5ErkJggg==);\
background-repeat:no-repeat;\
background-color:#f8f8f8}\
</style>";

void send_html(const char *h)
{
  char *r = (char *)_malloc(strlen(h) + strlen(html_header) + strlen(html_footer) + 16);
  sprintf(r, "%s%s%s", html_header, h, html_footer);
  server.send(200, "text/html", r);
#ifdef DEBUG_SEND
  Serial.println(r);
#endif
  free(r);
}

void handle_root()
{
  char *r = (char *)_malloc(strlen(html_css) + 118 + (10 * 619) + 11 + 44 + 44 + 29);

  int index = server.hasArg("index") ? atoi(server.arg("index").c_str()) : 0;
  int c = index;

  sprintf(r, "%s<table><thead><tr><th>Name</th><th>Button</th><th>Edit</th><th>Up</th><th>Down</th><th>Delete</th></tr></thead><tbody>", html_css);
  for (auto i = ir_codes.cbegin() + index; (i != ir_codes.cend()) && (c < (index + 10)); ++i, c++)
  {
    sprintf(r + strlen(r), "<tr><td data-label='Name'>%s</td>\
<td data-label='Button'><input type='button' class='wifi' onclick='window.location.href=\"/press?button=%d\";'/></td>\
<td data-label='Edit'><input type='button' class='edit' onclick='window.location.href=\"/edit?button=%d\";'/></td>\
<td data-label='Up'><input type='button' class='up' onclick='window.location.href=\"/up?button=%d\";'/></td>\
<td data-label='Down'><input type='button' class='down' onclick='window.location.href=\"/down?button=%d\";'/></td>\
<td data-label='Delete'><input type='button' class='trash' onclick='window.location.href=\"/del?button=%d\";'/></td>\
</tr>",
            (*i).name, c, c, c, c, c);
  }
  strcat(r, "<tfoot><tr>");
  if (index)
  {
    sprintf(r + strlen(r), "<td><a href='controle?index=%d'> < </a></td>", (index - 10));
  }
  if (((unsigned int)c < ir_codes.size()))
  {
    sprintf(r + strlen(r), "<td><a href='controle?index=%d'> > </a></td>", (index + 10));
  }
  strcat(r, "</tr></tfoot></tbody></table>");
  send_html(r);
  free(r);
}

void handle_add()
{
  char *r = (char *)_malloc(512);
  sprintf(r, "%s%s\
<form action='/save' method='post'>\
<p>1. Entre o nome do botão que você quer adicionar</p>\
<p>2. Espere a luz do WIFIIR começar a piscar</p>\
<p>3. Aperte o botão no controle até a luz desligar</p>\
<p>4. Clique em ADICIONAR abaixo</p>\
<input type='text' name='button' maxlength='31' value='%s'>\
<input type='submit' value='Salvar'>\
</form>",
          server.hasArg("empty") ? "<div class='card error'>Preencha o nome do botão</div>" : "",
          server.hasArg("nocode") ? "<div class='card error'>Erro na leitura do botão</div>" : "",
          server.hasArg("button") ? server.arg("button").c_str() : "");
  send_html(r);
  free(r);
  //inicia leitor de IR
  decoding_onoff = true;
}

void send_warning(const char *txt, int timeout = 500)
{
  char *r = (char *)_malloc(512);
  sprintf(r, "<div class='card warning'>%s</div><script>setTimeout(function (){document.location.href = '/controle';}, %d);</script>",
          txt, timeout);
  send_html(r);
  free(r);
}

void handle_save()
{
  String button = server.hasArg("button") ? server.arg("button") : "";
  if ((decoding_onoff) || (button.isEmpty()))
  {
    //redirect to /add with "button" as param
    char *r = (char *)_malloc(512);
    sprintf(r, "<script>document.location.href = '/add%s%s%s'</script>",
            button.isEmpty() ? "?empty=1" : "?button=",
            button.isEmpty() ? "" : button.c_str(),
            decoding_onoff ? "&nocode=1" : "");
    send_html(r);
    free(r);
  }
  else
  {
    //save button info
    strncpy(irresult.name, button.c_str(), sizeof(irresult.name) - 1);
    ir_codes.push_back(irresult);
    codes_save();
    send_warning("Botão Salvo!");
  }
}

void handle_press()
{
  ir_send(atoi(server.arg("button").c_str()));
  send_warning("Botão enviado");
}

void handle_edit()
{
  // ###
}

void move_button(int button, int offset)
{
  IrResult tmp;
  auto i = ir_codes.begin() + button;
  memcpy(&tmp, &(*i), sizeof(IrResult));
  ir_codes.erase(i);
  ir_codes.insert(ir_codes.begin() + button + offset, tmp);
  codes_save();
}

void handle_up()
{
  int button = atoi(server.arg("button").c_str());
  if (button != 0)
  {
    move_button(button, -1);
  }
  send_html("<script>document.location.href = '/controle'</script>");
}

void handle_down()
{
  int button = atoi(server.arg("button").c_str());
  if (button != (int)(ir_codes.size() - 1))
  {
    move_button(button, 1);
  }
  send_html("<script>document.location.href = '/controle'</script>");
}

void handle_del()
{
  ir_codes.erase(ir_codes.begin() + atoi(server.arg("button").c_str()));
  codes_save();
  send_warning("Botão deletado!");
}

void handle_config()
{
  char *r = (char *)_malloc(1024);
  strcpy(r, "<form action='/clear' method='post'>Apagar botões salvos:<input type='submit' value='Apagar'></form>\
<form action='/reboot' method='post'>Reiniciar WIFIIR:<input type='submit' value='Reiniciar'></form>\
<form action='/reset' method='post'>Limpar configurações WIFI:<input type='submit' value='Limpar'></form>");
#ifdef DEBUG_GENRND
  strcat(r, "</form>\
<form action='/random' method='post'>\
Gerar entradas aleatórias:<input type='submit' value='Gen'>\
</form>");
#endif
#ifdef SUPPORT_TELEGRAM
  sprintf(r + strlen(r), "<form action='/token' method='post'>\
Telegram Token:<input type='checkbox' onchange='document.getElementById(\"token\").disabled=!this.checked;\
document.getElementById(\"btoken\").disabled=!this.checked;'>\
<input type='text' id='token' name='token' disabled value='%s'/>\
<input type='submit' id='btoken' disabled value='Salvar'></form>",
          bt_token.c_str());
#endif
#ifdef SUPPORT_OTA
  strcat(r, "<form method='POST' action='/update' enctype='multipart/form-data'>\
Atualizar Firmware:<input type='file' accept='.bin,.bin.gz' name='firmware'>\
<input type='submit' value='Update Firmware'></form>");
#endif
  sprintf(r + strlen(r), "<br>Build Version: %s %s<br><br>", __DATE__, __TIME__);
  send_html(r);
  free(r);
}

#ifdef DEBUG_GENRND
void handle_random()
{
  IrResult tmp;
  memset(&tmp, 0, sizeof(IrResult));
  for (int i = 1; i <= DEBUG_GENRND; i++)
  {
    itoa(i, tmp.name, 10);
    ir_codes.push_back(tmp);
  }
  codes_save();
  send_warning("Entradas geradas!");
}
#endif

#ifdef SUPPORT_TELEGRAM
void handle_token()
{
  if (server.hasArg("token"))
  {
    bt_token = server.arg("token");
    bt_setup();
    telegram_save();
  }
  send_warning("Token Salvo!");
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
  send_warning("Botões limpos!");
}

void reboot()
{
  delay(1000);
  wm.reboot();
  delay(2000);
}

void handle_reset()
{
  send_warning("Reseting...", 5000);
  wm.resetSettings();
  reboot();
}

void handle_reboot()
{
  send_warning("Rebooting...", 5000);
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
  server.on("/edit", handle_edit);
  server.on("/up", handle_up);
  server.on("/down", handle_down);
  server.on("/del", handle_del);
#ifdef DEBUG_GENRND
  server.on("/random", handle_random);
#endif
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

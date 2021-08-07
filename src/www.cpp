/*
██╗    ██╗██╗    ██╗██╗    ██╗    ██╗  ██╗ █████╗ ███╗   ██╗██████╗ ██╗     ███████╗██████╗ ███████╗
██║    ██║██║    ██║██║    ██║    ██║  ██║██╔══██╗████╗  ██║██╔══██╗██║     ██╔════╝██╔══██╗██╔════╝
██║ █╗ ██║██║ █╗ ██║██║ █╗ ██║    ███████║███████║██╔██╗ ██║██║  ██║██║     █████╗  ██████╔╝███████╗
██║███╗██║██║███╗██║██║███╗██║    ██╔══██║██╔══██║██║╚██╗██║██║  ██║██║     ██╔══╝  ██╔══██╗╚════██║
╚███╔███╔╝╚███╔███╔╝╚███╔███╔╝    ██║  ██║██║  ██║██║ ╚████║██████╔╝███████╗███████╗██║  ██║███████║
 ╚══╝╚══╝  ╚══╝╚══╝  ╚══╝╚══╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝
*/

#include "www.h"

const char html_header[] PROGMEM = "\
<!DOCTYPE html><html lang='pt-br'>\
<head><title>WIFIIR</title>\
<meta charset='UTF-8'/>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<link rel='shortcut icon' href='data:image/x-icon;base64,iVBORw0KGgoAAAANSUhEUgAAABoAAAAaCAYAAACpSkzOAAAABmJLR0QA/wD/AP+gvaeTAAABoklEQVRIie3UvWsUURQF8F+2VEyKsJgIiokRApZa+VGIKVRSmVJtAgr6B1gEO/VvsLDWNkbUCKKFhbZpwoYIClZ20WiXj7WYu8zsM7M7E6JVDlyYefecc997986wj11ioE9+FNO4gFM4hqHI/cQ3LOMDXuJ73Q1cwiI20a4Ym3iFi1UKnMCbGuZlsRheO+Iq1hLBFj5iTnZ9EzgYcTLW5vApuEXtOq6nRe4lxC087bWrHTCBZ4nPNu53COOx0El+wemCwRHclV1HC78jWniNO8Hp4Ex4FIsdh0HZtLTxFsMhGMFjbOjfk43gHg7tcHi1w/tQZxdNnEcj3q/gR4UCaazhcng0wrOpBLP+Huv3uIVJ+TBM4nbk0jGfLTMv4rPufk1V0Ezha0G3WqXQwyC/kx97FI+whF8RS8EdCU5TfroHVQrBgcLzzTAu68s6bpRoK2NG9+iXxTau9TJq9ErinPzH2wqzoYgZrERuAGdrHqILR/ECT2TfW4rByC0Ed88wHqbPMbaXxikW5H2ZryPs16MU7Zr8XWNMdpJ5//jq9vH/8QdSN6mUF/XpPQAAAABJRU5ErkJggg=='/>\
<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/mini.css/3.0.1/mini-default.min.css'>\
<style>\
.w,.w:focus,.w:hover{\
background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAAmJLR0QA/4ePzL8AAAAHdElNRQflBxMQGDNbPF7PAAAA7klEQVQoz5XRPUuCAQDE8d+jUgRGWImbNFj2EEEIDS1NCdHiEkV9nfCrFEWLSwQ1tTi0BCFPL0u0iWEEQihqTVKUFd12x3+5O/5Q8MUl0PX2HUhaEJo2go4nkZrWABi1ZlnDtUctJGUtSrt0rh3I2vGsoi5txiSaHjRklKQcBPacuRBal1TXxKSMllORVcXAlBeb5py40jOGV3FLNtw5NhFg1oojI4ry4ui5daZjS9V9gJi+ebtqqhpIW7Fg342Y/qBuQR7k5EBeYdhQ20JEDoevmlI2Lqks9REmPgFtXSG62j99ESqhIvrtvMD/9A6R0TwXv22H+QAAACV0RVh0ZGF0ZTpjcmVhdGUAMjAyMS0wNy0xOVQxNjoyNDoyNSswMDowMHcnQy4AAAAldEVYdGRhdGU6bW9kaWZ5ADIwMjEtMDctMTlUMTY6MjQ6MjUrMDA6MDAGevuSAAAAAElFTkSuQmCC);\
background-repeat:no-repeat;\
background-color:#f8f8f8}\
.e,.e:focus,.e:hover{\
background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAAYQAAAGEBVSuQvQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAEKSURBVDiNldO9SgNBFAXgL0uw8AcUC5uAiKmshICdYOMjiNhaSCIWgr0voIW9T6CgjW1A8Aks7URQtBBBhARE1GImsI4TFw8sd4d775mzezg1AStYRM9PLOMCp4agHusCjjL9Ao9Yx0mOoIj1a8gFPVzjHht/KRiGc2zjFUt4x1lusFNBNEC7SkEDM5nFW7zkGFOCcUxl5p5iraWNIjk30MIz5nCJLh5yt+cUdOND+Ps5zOJN/KRUQRXmsSU4M/1fgqZg5x2usFNuVtnYxG58H8MxRssDbXn7csv75eWBLXWsYSKeR+JwH5tCqD6xh0Ol0P3yNUELk0JaP3AgSWxVFlajihshF/104BuZGSzACO/S3wAAAABJRU5ErkJggg==);\
background-repeat:no-repeat;\
background-color:#f8f8f8}\
.u,.u:focus,.u:hover{\
background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAAdAAAAHQBMYXlgQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAACVSURBVDiNY/z//z8DJYCJIt00N6AxkZGjMZGRA68J////x4qrUhjErXQYX1rpML6sSmEQx6UOqwuqUxnFD5xgvGSmxVJppsVSeeAE46XqVEZxolwAs7kgjDUJJlYQxpqEyyUENRMyhCjN+AwhWjMuQ0jSjM0QBnt9psekaEY2xF6f6TFDQwKDAKmaYdjPmUGccehnJgC+0ETc+bot3wAAAABJRU5ErkJggg==);\
background-repeat:no-repeat;\
background-color:#f8f8f8}\
.d,.d:focus,.d:hover{\
background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAAdAAAAHQBMYXlgQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAADmSURBVDiNxZM/isJAHIXfLzidhYUgITcIEa38B4JeYioN7CDYTilJZSF4DzsvobWVNl4hqK3lwrPaJYvJrtEFH0wz875vZmBGSOKVOC/R/yKYf0jlWXg+lSr6dSexWhmSKDKsVqZfdxLEIby2L5ciEquVaftyiUN4IFlIkoZJ4nshDuF1/pBYrUwnBf8QkEQ0gtsN5JwlsVqZbiDnaAQ3PX+3S5YkDyYJyXqJ8Vjc7V4OLb80A4Dd8XM5aLKxWDG5K+fdN5qg1gvk1AvkFE1Qy+tlnuAreihlAFhveM3r/Cp4JO//TDe1ACb01Xb7lgAAAABJRU5ErkJggg==);\
background-repeat:no-repeat;\
background-color:#f8f8f8}\
.t,.t:focus,.t:hover{\
background-image:url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAQAAAC1+jfqAAAABGdBTUEAALGPC/xhBQAAACBjSFJNAAB6JgAAgIQAAPoAAACA6AAAdTAAAOpgAAA6mAAAF3CculE8AAAAAmJLR0QA/4ePzL8AAAAHdElNRQflBxMQGDNbPF7PAAAA40lEQVQoz43RvU4CYRCF4Wd1WaJBIo00NhSa0EinlfRejom34S14F8TEzlKjrSbGgoLEn8TChg27LJ+FwLKGwlNMMfPOmclMZKHIuYaxSNuVpzLNoWOZHadu5GaObLkTq7n3EiN14trI87zpDew6c0uMkVcPPlW158CIDRDUtfTR1UVfS13gFwgKsaYeOjroaYoVwsIhVxMEVmJNvnBgKvFXiWkJ5GuBvAqESjlUgWytQ/avETGYSOZnj5YPSExKILdtbMj8nkNjibQEUvu+DPAIBmYa3i0t2y58K1Y22NR06YMfisxC2gggFqAAAAAldEVYdGRhdGU6Y3JlYXRlADIwMjEtMDctMTlUMTY6MjQ6MjUrMDA6MDB3J0MuAAAAJXRFWHRkYXRlOm1vZGlmeQAyMDIxLTA3LTE5VDE2OjI0OjI1KzAwOjAwBnr7kgAAAABJRU5ErkJggg==);\
background-repeat:no-repeat;\
background-color:#f8f8f8}\
.a{\
width:10%}\
.b,.g{\
width:20%}\
.g{\
text-align:center}\
.c{\
width:60%}\
.f{\
display:flex;\
flex-flow:row wrap;\
padding:0;\
overflow:hidden}\
.x{\
margin:0 auto;\
text-align:center}\
</style></head>\
<body><header class='sticky'>\
<div class='x card warning'>\
<p class='x'>WIFIIR - Controle remoto IR via WIFI</p>\
</div></header>\
";

const char html_footer[] PROGMEM = "\
<footer class='sticky'><label for='drawer-control' class='drawer-toggle'></label>\
<input type='checkbox' id='drawer-control' class='drawer'>\
<nav><label for='drawer-control' class='drawer-close'></label>\
<a class='doc' href='/'>Controle</a><br>\
<a class='doc' href='/a'>Adicionar</a><br>\
<a class='doc' href='/g'>Configurações</a><br>\
</nav></footer></body></html>";

void send_html(const char *h)
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send_P(200, "text/html", html_header);
  String s = "WIFIIR" + (wifiir_subname.isEmpty() ? "" : ("-" + wifiir_subname));
  server.sendContent("<div class='x card'><p class='x'>" + s + "</p></div><br>");
  server.sendContent(h);
  server.sendContent_P(html_footer);
#ifdef DEBUG_SEND
  _Serial.println(h);
#endif
}

void handle_root()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  char *r = (char *)_malloc(15 + (10 * (552 + 32)) + 15 + 24 + 42 + 24 + 12);

  int index = server.hasArg("i") ? atoi(server.arg("i").c_str()) : 0;

  //limit index 0-max
  index = (index < (int)ir_codes.size()) ? index : ((int)ir_codes.size() - 10);
  index = (index < 0) ? 0 : index;

  int c = index;

  //table
  strcpy(r, "<div class='f'>");
  for (auto i = ir_codes.cbegin() + index; (i != ir_codes.cend()) && (c < (index + 10)); ++i, c++)
  {
    sprintf_P(r + strlen(r), PSTR("\
<div class='a'></div>\
<div style='width:40%%'>%s</div>\
<div class='a'><input type='button' class='w' onclick='window.location.href=\"/p?b=%d\";'/></div>\
<div class='a'><input type='button' class='e' onclick='window.location.href=\"/e?b=%d\";'/></div>\
<div class='a'><input type='button' class='u' onclick='window.location.href=\"/u?b=%d\";'/></div>\
<div class='a'><input type='button' class='d' onclick='window.location.href=\"/d?b=%d\";'/></div>\
<div class='a'><input type='button' class='t' onclick='window.location.href=\"/t?b=%d\";'/></div>\
"),
              (*i).name, c, c, c, c, c);
  }

  //nav
  strcat(r, "<div class='g'>");
  if (index)
  {
    sprintf(r + strlen(r), "<a href='/?i=%d'> < </a>", (index - 10));
  }
  strcat(r, "</div><div class='c'></div><div class='g'>");
  if (((unsigned int)c < ir_codes.size()))
  {
    sprintf(r + strlen(r), "<a href='/?i=%d'> > </a>", (index + 10));
  }
  strcat(r, "</div></div>");

  send_html(r);
  free(r);
}

void send_warning(const char *txt, int timeout = 500)
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  char *r = (char *)_malloc(512);
  sprintf_P(r, PSTR("<div class='x card warning'><p class='x'>%s</p></div><script>setTimeout(function (){document.location.href = '/';}, %d);</script>"),
            txt, timeout);
  send_html(r);
  free(r);
}

void handle_add()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  char *r = (char *)_malloc(1024);
  sprintf_P(r, PSTR("%s%s\
<form action='/s' method='POST'>\
<p>1. Entre o nome do botão que você quer adicionar</p>\
<input type='text' name='b' maxlength='31' value='%s'>\
<p>2. Espere a luz do WIFIIR começar a piscar</p>\
<p>3. Aperte o botão no controle até a luz desligar</p>\
<p>4. Clique em ADICIONAR abaixo</p>\
<input type='submit' value='Adicionar'>\
</form>"),
            server.hasArg("e") ? "<div class='x card error'><p class='x'>Preencha o nome do botão</p></div>" : "",
            server.hasArg("n") ? "<div class='x card error'><p class='x'>Erro na leitura do botão</p></div>" : "",
            server.hasArg("b") ? server.arg("b").c_str() : "");
  send_html(r);
  free(r);
  //inicia leitor de IR
  decoding_onoff = true;
}

void handle_add2()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  String button = server.hasArg("b") ? server.arg("b") : "";
  //clicou add sem ter nome/codigo (ou passou muito tempo)
  if ((decoding_onoff) || (irin_timeout) || (button.isEmpty()))
  {
    //redirect to /add with "button" as param
    char *r = (char *)_malloc(512);
    sprintf(r, "<script>document.location.href = '/a%s%s%s%s'</script>",
            button.isEmpty() ? "?e=1" : "?b=",
            button.isEmpty() ? "" : button.c_str(),
            decoding_onoff ? "&n=1" : "",
            irin_timeout ? "&n=1" : "");
    send_html(r);
    free(r);
  }
  else
  {
    //save button info
    irresult.type = IR_CODE;
    strncpy(irresult.name, button.c_str(), sizeof(irresult.name) - 1);
    ir_codes.push_back(irresult);
    codes_save();
    send_warning("Botão Salvo!");
  }
}

void handle_press()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  ir_send(atoi(server.arg("b").c_str()));
  send_warning("Botão enviado");
}

void handle_edit()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  int button = atoi(server.arg("b").c_str());
  auto i = ir_codes.cbegin() + button;
  if (server.hasArg("n"))
  {
    //chamou com nome, modifica e salva
    strcpy((char *)(*i).name, server.arg("n").c_str());
    codes_save();
    send_warning("Botão editado!");
  }
  else
  {
    //chamou sem nome, pergunta
    char *r = (char *)_malloc(512);
    sprintf_P(r, PSTR("\
<form class='f' action='/e' method='POST'>\
<div class='b'>Novo nome:</div><div class='c'>\
<input type='hidden' name='b' value='%d'>\
<input type='text' name='n' maxlength='31' value='%s'></div><div class='b'>\
<input type='submit' value='Salvar'>\
</div></form>"),
              button, (*i).name);
    send_html(r);
    free(r);
  }
}

void move_button(int button, int offset)
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  CODES tmp;
  auto i = ir_codes.begin() + button;
  memcpy(&tmp, &(*i), sizeof(CODES));
  ir_codes.erase(i);
  ir_codes.insert(ir_codes.begin() + button + offset, tmp);
  codes_save();
}

void _main_with_index(int b)
{
  //round to start of page
  b /= 10;
  b *= 10;
  String s = "<script>document.location.href = '/?i=";
  s += String(b);
  s += "'</script>";
  send_html(s.c_str());
}

void handle_up()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  int button = atoi(server.arg("b").c_str());
  //primeiro botão nao pode ir pra cima
  if (button != 0)
  {
    move_button(button, -1);
  }
  _main_with_index(button);
}

void handle_down()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  int button = atoi(server.arg("b").c_str());
  //ultimo botão não pode ir pra baixo
  if (button != (int)(ir_codes.size() - 1))
  {
    move_button(button, 1);
  }
  _main_with_index(button);
}

void handle_del()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  int button = atoi(server.arg("b").c_str());
  ir_codes.erase(ir_codes.begin() + button);
  codes_save();
  _main_with_index(button);
}

void handle_config()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  char *r = (char *)_malloc(2048);//218+32+512+170+400+64+169+172+265+28+20
  sprintf_P(r, PSTR("\
<form class='f' action='/w' method='POST'><div class='b'>Device name</div>\
<div class='c'><input type='text' name='n' maxlength='31' value='%s'/></div>\
<div class='b'><input type='submit' value='Salvar'></div></form>"),
            wifiir_subname.c_str());
  strcat_P(r, PSTR("\
<form class='f' action='/b' method='POST'><div class='b'>Reiniciar WIFIIR</div><div class='c'></div><div class='b'><input type='submit' value='Reiniciar'></div></form>\
<form class='f' action='/r' method='POST'><div class='b'>Limpar configurações WIFI</div><div class='c'></div><div class='b'><input type='submit' value='Limpar'></div></form>\
<form class='f' action='/l' method='POST'><div class='b'>Apagar botões salvos</div><div class='c'></div><div class='b'><input type='submit' value='Apagar'></div></form>"));
#ifdef DEBUG_GENRNDBTN
  strcat_P(r, PSTR("\
<form class='f' action='/y' method='POST'><div class='b'>Gerar botões aleatórias</div><div class='c'></div><div class='b'><input type='submit' value='Gerar'></div></form>"));
#endif
#ifdef SUPPORT_TELEGRAM
  sprintf_P(r + strlen(r), PSTR("\
<form class='f' action='/k' method='POST'><div class='b'>Telegram Token</div>\
<div class='c'><input type='checkbox' onchange='document.getElementById(\"k\").disabled=!this.checked;document.getElementById(\"bk\").disabled=!this.checked;'>\
<input type='text' id='k' name='k' disabled value='%s' maxlength='63'/></div>\
<div class='b'><input type='submit' id='bk' disabled value='Salvar'></div></form>"),
            bt_token.c_str());
  strcat_P(r, PSTR("\
<form class='f' action='/x' method='POST'><div class='b'>Gerenciar usuários</div><div class='c'></div><div class='b'><input type='submit' value='Gerenciar'></div></form>"));
#ifdef DEBUG_GENRNDUSR
  strcat_P(r, PSTR("\
<form class='f' action='/z' method='POST'><div class='b'>Gerar usuários aleatórios</div><div class='c'></div><div class='b'><input type='submit' value='Gerar'></div></form>"));
#endif
#endif
#ifdef SUPPORT_OTA
  strcat_P(r, PSTR("\
<form class='f' method='POST' action='/update' enctype='multipart/form-data'>\
<div class='b'>Atualizar Firmware</div><div class='c'><input type='file' accept='.bin,.bin.gz' name='firmware'></div><div class='b'>\
<input type='submit' value='Atualizar'></div></form>"));
#endif
  sprintf(r + strlen(r), "<br>Build Version: %s %s<br><br>", __DATE__, __TIME__);
  send_html(r);
  free(r);
}

#ifdef DEBUG_GENRNDBTN
void handle_randombtn()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  CODES tmp;
  memset(&tmp, 0, sizeof(CODES));
  //gera botões aleatorios
  for (int i = 1; i <= DEBUG_GENRNDBTN; i++)
  {
    itoa(i, tmp.name, 10);
    ir_codes.push_back(tmp);
  }
  codes_save();
  send_warning("Entradas geradas!");
}
#endif

#ifdef SUPPORT_TELEGRAM

#ifdef DEBUG_GENRNDUSR
void handle_randomusr()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  TUSERS tmp;
  //gera botões aleatorios
  for (int i = 1; i <= DEBUG_GENRNDUSR; i++)
  {
    tmp.auth = false;
    tmp.id = rand();
    itoa(tmp.id, tmp.name, 16);
#ifdef DEBUG
    _Serial.printf("BT_RND: %d\t%d\t%s\n", tmp.auth, tmp.id, tmp.name);
#endif
    bt_users.push_back(tmp);
  }
  send_warning("Usuarios geradas!");
}
#endif

void handle_token()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  bt_token = server.hasArg("k") ? server.arg("k") : "";
  bt_setup();
  telegram_save();
  tb_kbd();
  send_warning("Token Salvo!");
}

void handle_name()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  wifiir_subname = server.hasArg("n") ? server.arg("n") : "";
  wifiir_name_save();
  send_warning("Nome Salvo!");
}

void handle_userman()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  if (!server.hasArg("w"))
  {
    //build table
    char *r = (char *)_malloc(42 + ((165 + 8 + 32 + 7) * bt_users.size()) + 181);

    strcpy(r, "<form class='f' action='/x' method='POST'>");
    int c = 0;
    for (auto i = bt_users.cbegin(); i != bt_users.cend(); ++i, c++)
    {
      sprintf_P(r + strlen(r), PSTR("<div class='b'></div><div class='b'><input type='checkbox' name='%d' value='1' %s></div>\
<div class='b'>%08x</div><div class='b'>%s</div><div class='b'></div>"),
                c,
                (*i).auth ? "checked" : "",
                (*i).id, (*i).name);
    }
    strcat_P(r, PSTR("<div class='b'></div><div class='b'></div>\
<input class='b' name='w' type='hidden' value='1'><input type='submit' value='Salvar'>\
<div class='b'></div><div class='b'></div></form>"));

    send_html(r);
    free(r);
  }
  else
  {
    //manage results
    std::vector<TUSERS> tmp_users;
    int c = 0;
    for (auto i = bt_users.cbegin(); i != bt_users.cend(); ++i, c++)
    {
      TUSERS tmp;
      tmp.auth = server.hasArg(String(c)) ? true : false;
      tmp.id = (*i).id;
      strcpy(tmp.name, (*i).name);
      tmp_users.push_back(tmp);
    }
    bt_users.clear();
    bt_users.swap(tmp_users);

    telegram_users_save();
    send_warning("Usuários salvos!");
  }
}
#endif

void handle_clear()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
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
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  delay(1000);
  ESP.restart();
  delay(2000);
}

void handle_reset()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  send_warning("Reseting...", 5000);
  wm.resetSettings();
  reboot();
}

void handle_reboot()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  send_warning("Rebooting...", 5000);
  reboot();
}

void handle_404()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  send_html("<p>Not found!</p>");
}

#ifdef DEBUG_FS
void handle_files()
{
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  //download
  if (server.hasArg("n"))
  {
    String fname = server.arg("n");
    char buf[512];
    int r;
    server.send(200, "application/octet-stream", "");
#ifdef SUPPORT_LITTLEFS
    File f = LittleFS.open(fname.c_str(), "r");
#else
    File f = SPIFFS.open(fname.c_str(), "r");
#endif
    do
    {
      r = f.read((uint8_t *)&buf, sizeof(buf));
      server.sendContent(buf, r);
    } while (r == sizeof(buf));
    f.close();
  }
  else if (server.hasArg("x"))
  //delete
  {
    String fname = server.arg("x");
#ifdef SUPPORT_LITTLEFS
    LittleFS.remove(fname.c_str());
#else
    SPIFFS.remove(fname.c_str());
#endif
    server.send(200, "text/html", "<script>document.location.href = '/f'</script>");
  }
  else
  //dir list
  {
    String s;
    char buf[16];
    server.send(200, "text/html", "");
#ifdef SUPPORT_LITTLEFS
    Dir dir = LittleFS.openDir("/");
#else
    Dir dir = SPIFFS.openDir("/");
#endif
    while (dir.next())
    {
      s = "<a download='" + dir.fileName() + "' href='f?n=" + dir.fileName() + "'>" + dir.fileName() + "</a>";
      itoa(dir.fileSize(), buf, 10);
      s += "    (" + String(buf) + ")    ";
      const time_t t = dir.fileTime();
      s += String(ctime(&t));
      s += "<a href='f?x=" + dir.fileName() + "'>x</a><br>";
      server.sendContent(s.c_str());
    }
    server.sendContent("<br><br><a href='/'>BACK</a><br>");
  }
}
#endif

void handle_rclient()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
#ifdef DEBUG
  _Serial.println("!Remote client connected");
#endif
  server.send(200, "text/html", "OK");
}

void install_www_handlers()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif

  //abcdefgHIJklMNOpQrstuVwxyz
  //description.xml
  server.on("/", handle_root);
  server.on("/a", handle_add);
  server.on("/p", handle_press);
  server.on("/e", handle_edit);
  server.on("/u", handle_up);
  server.on("/d", handle_down);
  server.on("/t", handle_del);
#ifdef DEBUG_GENRNDBTN
  server.on("/y", handle_randombtn);
#endif
#ifdef SUPPORT_TELEGRAM
  server.on("/k", handle_token);
  server.on("/x", handle_userman);
#ifdef DEBUG_GENRNDUSR
  server.on("/z", handle_randomusr);
#endif
#endif
  server.on("/s", handle_add2);
  server.on("/b", handle_reboot);
  server.on("/r", handle_reset);
  server.on("/g", handle_config);
  server.on("/l", handle_clear);
  server.on("/w", handle_name);
#ifdef SUPPORT_SSDP
  server.on("/description.xml", HTTP_GET, []()
            { SSDP_esp8266.schema(server.client()); });
#endif
#ifdef DEBUG_FS
  server.on("/f", handle_files);
#endif
  server.on("/c", handle_rclient);
  server.onNotFound(handle_404);
}

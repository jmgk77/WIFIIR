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
<head><meta charset='UTF-8'/>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<link rel='shortcut icon' href='icon.png' type='image/png'/>\
<link rel='stylesheet' href='mini-default.min.css'>\
<link rel='stylesheet' href='wifiir.css'>\
<title>WIFIIR</title></head>\
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

/*
██████╗  ██████╗  ██████╗ ████████╗
██╔══██╗██╔═══██╗██╔═══██╗╚══██╔══╝
██████╔╝██║   ██║██║   ██║   ██║
██╔══██╗██║   ██║██║   ██║   ██║
██║  ██║╚██████╔╝╚██████╔╝   ██║
╚═╝  ╚═╝ ╚═════╝  ╚═════╝    ╚═╝
*/

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
              i->name, c, c, c, c, c);
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

/*
 █████╗ ██████╗ ██████╗
██╔══██╗██╔══██╗██╔══██╗
███████║██║  ██║██║  ██║
██╔══██║██║  ██║██║  ██║
██║  ██║██████╔╝██████╔╝
╚═╝  ╚═╝╚═════╝ ╚═════╝
*/

void handle_add()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  char *r = (char *)_malloc(1024);
  sprintf_P(r, PSTR("%s%s\
<script>window.onbeforeunload=function(){a=new XMLHttpRequest();a.open('GET', \"/v\", true);a.send();};\
setInterval(function(){document.location.href=\"/\";},120000);</script>\
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
  button.trim();
  //clicou add sem ter nome/codigo (ou passou muito tempo)
  if ((decoding_onoff) || (irin_timeout) || (button.isEmpty()))
  {
    //redirect to /add with "button" as param
    char *r = (char *)_malloc(512);
    sprintf(r, "<script>document.location.href = '/a%s%s%s'</script>",
            button.isEmpty() ? "?e=1" : "?b=",
            button.isEmpty() ? "" : button.c_str(),
            (decoding_onoff || irin_timeout) ? "&n=1" : "");
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
    String n = server.arg("n");
    n.trim();
    strcpy((char *)i->name, n.c_str());
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
              button, i->name);
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

/*
 ██████╗ ██████╗ ███╗   ██╗███████╗██╗ ██████╗
██╔════╝██╔═══██╗████╗  ██║██╔════╝██║██╔════╝
██║     ██║   ██║██╔██╗ ██║█████╗  ██║██║  ███╗
██║     ██║   ██║██║╚██╗██║██╔══╝  ██║██║   ██║
╚██████╗╚██████╔╝██║ ╚████║██║     ██║╚██████╔╝
 ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝ ╚═════╝
*/

void handle_config()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  char *r = (char *)_malloc(218 + 32 + 517 + 170 + 454 + 64 + 185 + 172 + 265 + 28 + 20 + 61);
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
<div class='c'><input type='checkbox' onchange='\
document.getElementById(\"a\").disabled=!this.checked;document.getElementById(\"b\").disabled=!this.checked;document.getElementById(\"c\").disabled=!this.checked;\
'><input type='text' id='a' name='k' disabled value='%s' maxlength='63'/></div>\
<div class='b'><input type='submit' id='b' disabled value='Salvar'></div></form>"),
            bt_token.c_str());
  strcat_P(r, PSTR("\
<form class='f' action='/x' method='POST'><div class='b'>Gerenciar usuários</div><div class='c'></div><div class='b'><input type='submit' id='c' disabled value='Gerenciar'></div></form>"));
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
#ifdef DEBUG
  sprintf(r + strlen(r), "<a href='/c'>HW Info</a><br><a href='/f'>File browser</a><br>");
#endif
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
  bt_token.trim();
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
  wifiir_subname.trim();
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
                i->auth ? "checked" : "",
                i->id, i->name);
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
      tmp.id = i->id;
      strcpy(tmp.name, i->name);
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
  LittleFS.remove("/codes.bin");
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

void handle_info()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  FSInfo fs_info;
  LittleFS.info(fs_info);

  char *buf = (char *)_malloc(512);
  sprintf_P(buf, PSTR("OK\n\nBuild Version: %s %s\nReset: %s\nLocal IP: %s\nBoot time: %s\n\
ESP8266_INFO\ngetBootMode: %d\ngetSdkVersion: %s\ngetBootVersion: %d\ngetChipId: %08x\n\
getFlashChipSize: %d\ngetFlashChipRealSize: %d\ngetFlashChipSizeByChipId: %d\ngetFlashChipId: %08x\n\
getFreeHeap: %d\n\nFS_INFO\ntotalBytes: %d\nusedBytes: %d\nblockSize: %d\npageSize: %d\n\
maxOpenFiles: %d\nmaxPathLength: %d\n\n"),
            WIFIIR_VERSION,
            ESP.getResetReason().c_str(), WiFi.localIP().toString().c_str(), boot_time.c_str(),
            ESP.getBootMode(), ESP.getSdkVersion(), ESP.getBootVersion(), ESP.getChipId(), ESP.getFlashChipSize(),
            ESP.getFlashChipRealSize(), ESP.getFlashChipSizeByChipId(), ESP.getFlashChipId(), ESP.getFreeHeap(),
            fs_info.totalBytes, fs_info.usedBytes, fs_info.blockSize, fs_info.pageSize, fs_info.maxOpenFiles, fs_info.maxPathLength);
  server.send(200, "text/txt", buf);
  free(buf);
}

void handle_timeout()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  _end_ir();
  server.send(200, "text/txt", "OK");
}

/*
███████╗███████╗
██╔════╝██╔════╝
█████╗  ███████╗
██╔══╝  ╚════██║
██║     ███████║
╚═╝     ╚══════╝
*/

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
    File f = LittleFS.open(fname, "r");
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
    LittleFS.remove(fname);
    server.send(200, "text/html", "<script>document.location.href = '/f'</script>");
  }
  else
  //dir list
  {
    String s;
    char buf[16];
    server.send(200, "text/html", "");
    Dir dir = LittleFS.openDir("/");

    while (dir.next())
    {
      if (dir.isFile())
      {
        s = "<a download='" + dir.fileName() + "' href='f?n=" + dir.fileName() + "'>" + dir.fileName() + "</a>";
        itoa(dir.fileSize(), buf, 10);
        s += "    (" + String(buf) + ")    ";
        const time_t t = dir.fileTime();
        s += String(ctime(&t));
        s += "<a href='f?x=" + dir.fileName() + "'>x</a><br>";
        server.sendContent(s);
      }
    }
    server.sendContent("<br><br><a href='/'>BACK</a><br>");
  }
}
#endif

/*
 ██████╗ ███████╗███╗   ██╗███████╗██████╗ ██╗ ██████╗
██╔════╝ ██╔════╝████╗  ██║██╔════╝██╔══██╗██║██╔════╝
██║  ███╗█████╗  ██╔██╗ ██║█████╗  ██████╔╝██║██║
██║   ██║██╔══╝  ██║╚██╗██║██╔══╝  ██╔══██╗██║██║
╚██████╔╝███████╗██║ ╚████║███████╗██║  ██║██║╚██████╗
 ╚═════╝ ╚══════╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝╚═╝ ╚═════╝
*/

bool handle_generic(String path)
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  if (LittleFS.exists(path))
  {
    File f = LittleFS.open(path, "r");
    server.streamFile(f, path.endsWith(".css") ? "text/css" : "image/png");
    f.close();
    return true;
  }
  return false;
}

void handle_404()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif
  if (!handle_generic("www" + server.uri()))
  {
    server.send(404, "text/plain", "Not found!");
  }
}

/*
██╗███╗   ██╗███████╗████████╗ █████╗ ██╗     ██╗
██║████╗  ██║██╔════╝╚══██╔══╝██╔══██╗██║     ██║
██║██╔██╗ ██║███████╗   ██║   ███████║██║     ██║
██║██║╚██╗██║╚════██║   ██║   ██╔══██║██║     ██║
██║██║ ╚████║███████║   ██║   ██║  ██║███████╗███████╗
╚═╝╚═╝  ╚═══╝╚══════╝   ╚═╝   ╚═╝  ╚═╝╚══════╝╚══════╝
*/

void install_www_handlers()
{
#ifdef DEBUG_F
  _Serial.println(__func__);
#endif

  //abcdefgHIJklMNOpQrstuvwxyz
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
  server.on("/c", handle_info);
  server.on("/v", handle_timeout);
  server.onNotFound(handle_404);
}

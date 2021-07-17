//WIFIIR - Controlador IR via internet
//
//(c) JMGK 2021

#define DEBUG 1

#ifndef DEBUG
#define _malloc malloc
#endif

#include <Arduino.h>

#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

#include <LittleFS.h>

#include <IRrecv.h>

//irRecv vars
const uint16_t kRecvPin = D7;
const uint16_t kCaptureBufferSize = 1024;
const uint8_t kTimeout = 50;
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);
bool irin_enable = false;

//
typedef struct
{
  char name[32];
  decode_type_t protocol;
  uint16_t size;
  uint64_t value;
} IrResult;
IrResult irresult;

std::vector<IrResult> ir_codes;

decode_results results;

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
██████╗ ███████╗██████╗ ██╗   ██╗ ██████╗
██╔══██╗██╔════╝██╔══██╗██║   ██║██╔════╝
██║  ██║█████╗  ██████╔╝██║   ██║██║  ███╗
██║  ██║██╔══╝  ██╔══██╗██║   ██║██║   ██║
██████╔╝███████╗██████╔╝╚██████╔╝╚██████╔╝
╚═════╝ ╚══════╝╚═════╝  ╚═════╝  ╚═════╝
*/

#ifdef DEBUG
void dump_ir(IrResult irresult)
{
  String s;
  switch (irresult.protocol)
  {
  case UNKNOWN:
    s = "UNKNOWN";
    break;
  case UNUSED:
    s = "UNUSED";
    break;
  case RC5:
    s = "RC5";
    break;
  case RC6:
    s = "RC6";
    break;
  case NEC:
    s = "NEC";
    break;
  case SONY:
    s = "SONY";
    break;
  case PANASONIC:
    s = "PANASONIC";
    break;
  case JVC:
    s = "JVC";
    break;
  case SAMSUNG:
    s = "SAMSUNG";
    break;
  case WHYNTER:
    s = "WHYNTER";
    break;
  case AIWA_RC_T501:
    s = "AIWA_RC_T501";
    break;
  case LG:
    s = "LG";
    break;
  case SANYO:
    s = "SANYO";
    break;
  case MITSUBISHI:
    s = "MITSUBISHI";
    break;
  case DISH:
    s = "DISH";
    break;
  case SHARP:
    s = "SHARP";
    break;
  case COOLIX:
    s = "COOLIX";
    break;
  case DAIKIN:
    s = "DAIKIN";
    break;
  case DENON:
    s = "DENON";
    break;
  case KELVINATOR:
    s = "KELVINATOR";
    break;
  case SHERWOOD:
    s = "SHERWOOD";
    break;
  case MITSUBISHI_AC:
    s = "MITSUBISHI_AC";
    break;
  case RCMM:
    s = "RCMM";
    break;
  case SANYO_LC7461:
    s = "SANYO_LC7461";
    break;
  case RC5X:
    s = "RC5X";
    break;
  case GREE:
    s = "GREE";
    break;
  case PRONTO:
    s = "PRONTO";
    break;
  case NEC_LIKE:
    s = "NEC_LIKE";
    break;
  case ARGO:
    s = "ARGO";
    break;
  case TROTEC:
    s = "TROTEC";
    break;
  case NIKAI:
    s = "NIKAI";
    break;
  case RAW:
    s = "RAW";
    break;
  case GLOBALCACHE:
    s = "GLOBALCACHE";
    break;
  case TOSHIBA_AC:
    s = "TOSHIBA_AC";
    break;
  case FUJITSU_AC:
    s = "FUJITSU_AC";
    break;
  case MIDEA:
    s = "MIDEA";
    break;
  case MAGIQUEST:
    s = "MAGIQUEST";
    break;
  case LASERTAG:
    s = "LASERTAG";
    break;
  case CARRIER_AC:
    s = "CARRIER_AC";
    break;
  case HAIER_AC:
    s = "HAIER_AC";
    break;
  case MITSUBISHI2:
    s = "MITSUBISHI2";
    break;
  case HITACHI_AC:
    s = "HITACHI_AC";
    break;
  case HITACHI_AC1:
    s = "HITACHI_AC1";
    break;
  case HITACHI_AC2:
    s = "HITACHI_AC2";
    break;
  case GICABLE:
    s = "GICABLE";
    break;
  case HAIER_AC_YRW02:
    s = "HAIER_AC_YRW02";
    break;
  case WHIRLPOOL_AC:
    s = "WHIRLPOOL_AC";
    break;
  case SAMSUNG_AC:
    s = "SAMSUNG_AC";
    break;
  case LUTRON:
    s = "LUTRON";
    break;
  case ELECTRA_AC:
    s = "ELECTRA_AC";
    break;
  case PANASONIC_AC:
    s = "PANASONIC_AC";
    break;
  case PIONEER:
    s = "PIONEER";
    break;
  case LG2:
    s = "LG2";
    break;
  case MWM:
    s = "MWM";
    break;
  case DAIKIN2:
    s = "DAIKIN2";
    break;
  case VESTEL_AC:
    s = "VESTEL_AC";
    break;
  case TECO:
    s = "TECO";
    break;
  case SAMSUNG36:
    s = "SAMSUNG36";
    break;
  case TCL112AC:
    s = "TCL112AC";
    break;
  case LEGOPF:
    s = "LEGOPF";
    break;
  case MITSUBISHI_HEAVY_88:
    s = "MITSUBISHI_HEAVY_88";
    break;
  case MITSUBISHI_HEAVY_152:
    s = "MITSUBISHI_HEAVY_152";
    break;
  case DAIKIN216:
    s = "DAIKIN216";
    break;
  case SHARP_AC:
    s = "SHARP_AC";
    break;
  case GOODWEATHER:
    s = "GOODWEATHER";
    break;
  case INAX:
    s = "INAX";
    break;
  case DAIKIN160:
    s = "DAIKIN160";
    break;
  case NEOCLIMA:
    s = "NEOCLIMA";
    break;
  case DAIKIN176:
    s = "DAIKIN176";
    break;
  case DAIKIN128:
    s = "DAIKIN128";
    break;
  case AMCOR:
    s = "AMCOR";
    break;
  case DAIKIN152:
    s = "DAIKIN152";
    break;
  case MITSUBISHI136:
    s = "MITSUBISHI136";
    break;
  case MITSUBISHI112:
    s = "MITSUBISHI112";
    break;
  case HITACHI_AC424:
    s = "HITACHI_AC424";
    break;
  case SONY_38K:
    s = "SONY_38K";
    break;
  case EPSON:
    s = "EPSON";
    break;
  case SYMPHONY:
    s = "SYMPHONY";
    break;
  case HITACHI_AC3:
    s = "HITACHI_AC3";
    break;
  case DAIKIN64:
    s = "DAIKIN64";
    break;
  case AIRWELL:
    s = "AIRWELL";
    break;
  case DELONGHI_AC:
    s = "DELONGHI_AC";
    break;
  case DOSHISHA:
    s = "DOSHISHA";
    break;
  case MULTIBRACKETS:
    s = "MULTIBRACKETS";
    break;
  case CARRIER_AC40:
    s = "CARRIER_AC40";
    break;
  case CARRIER_AC64:
    s = "CARRIER_AC64";
    break;
  case HITACHI_AC344:
    s = "HITACHI_AC344";
    break;
  case CORONA_AC:
    s = "CORONA_AC";
    break;
  case MIDEA24:
    s = "MIDEA24";
    break;
  case ZEPEAL:
    s = "ZEPEAL";
    break;
  case SANYO_AC:
    s = "SANYO_AC";
    break;
  case VOLTAS:
    s = "VOLTAS";
    break;
  case METZ:
    s = "METZ";
    break;
  case TRANSCOLD:
    s = "TRANSCOLD";
    break;
  case TECHNIBEL_AC:
    s = "TECHNIBEL_AC";
    break;
  case MIRAGE:
    s = "MIRAGE";
    break;
  case ELITESCREENS:
    s = "ELITESCREENS";
    break;
  case PANASONIC_AC32:
    s = "PANASONIC_AC32";
    break;
  case MILESTAG2:
    s = "MILESTAG2";
    break;
  case ECOCLIM:
    s = "ECOCLIM";
    break;
  case XMP:
    s = "XMP";
    break;
  case TRUMA:
    s = "TRUMA";
    break;
  case HAIER_AC176:
    s = "HAIER_AC176";
    break;
  case TEKNOPOINT:
    s = "TEKNOPOINT";
    break;
  case KELON:
    s = "KELON";
    break;
  default:
    s = "ERROR!";
  }
  Serial.print("PROTOCOL: ");
  Serial.println(s.c_str());
  Serial.print("VALUE: ");
  Serial.println(irresult.value);
  Serial.print("SIZE: ");
  Serial.println(irresult.size);
}

void _hexdump(void *ptr, int buflen)
{
  unsigned char *buf = (unsigned char *)ptr;
  int i, j;
  char s[128];
  for (i = 0; i < buflen; i += 16)
  {
    sprintf(s, "%06x: ", i);
    Serial.print(s);
    for (j = 0; j < 16; j++)
      if (i + j < buflen)
      {
        sprintf(s, "%02x ", buf[i + j]);
        Serial.print(s);
      }
      else
      {
        sprintf(s, "   ");
        Serial.print(s);
      }
    sprintf(s, " ");
    Serial.print(s);
    for (j = 0; j < 16; j++)
      if (i + j < buflen)
      {
        sprintf(s, "%c", isprint(buf[i + j]) ? buf[i + j] : '.');
        Serial.print(s);
      }
    sprintf(s, "\n");
    Serial.print(s);
  }
}

void dump_fs()
{
  FSInfo fs_info;
  LittleFS.info(fs_info);
  Serial.printf("FS_INFO\ntotalBytes: %d\nusedBytes: %d\nblockSize: %d\npageSize: %d\nmaxOpenFiles: %d\nmaxPathLength: %d\n",
                fs_info.totalBytes, fs_info.usedBytes, fs_info.blockSize, fs_info.pageSize, fs_info.maxOpenFiles, fs_info.maxPathLength);
}

void dump_esp8266()
{
  Serial.println("ESP8266_INFO");
  Serial.print("ESP.getBootMode(); ");
  Serial.println(ESP.getBootMode());
  Serial.print("ESP.getSdkVersion(); ");
  Serial.println(ESP.getSdkVersion());
  Serial.print("ESP.getBootVersion(); ");
  Serial.println(ESP.getBootVersion());
  Serial.print("ESP.getChipId(); ");
  Serial.println(ESP.getChipId());
  Serial.print("ESP.getFlashChipSize(); ");
  Serial.println(ESP.getFlashChipSize());
  Serial.print("ESP.getFlashChipRealSize(); ");
  Serial.println(ESP.getFlashChipRealSize());
  Serial.print("ESP.getFlashChipSizeByChipId(); ");
  Serial.println(ESP.getFlashChipSizeByChipId());
  Serial.print("ESP.getFlashChipId(); ");
  Serial.println(ESP.getFlashChipId());
  Serial.print("ESP.getFreeHeap(); ");
  Serial.println(ESP.getFreeHeap());
}

void *_malloc(size_t size)
{
  Serial.printf("MALLOC: asked %d available (%d)\n", size, ESP.getFreeHeap());
  return malloc(size);
}

#endif

/*
██████╗ ███████╗██████╗ ███████╗██╗███████╗████████╗ █████╗ ███╗   ██╗ ██████╗███████╗
██╔══██╗██╔════╝██╔══██╗██╔════╝██║██╔════╝╚══██╔══╝██╔══██╗████╗  ██║██╔════╝██╔════╝
██████╔╝█████╗  ██████╔╝███████╗██║███████╗   ██║   ███████║██╔██╗ ██║██║     █████╗
██╔═══╝ ██╔══╝  ██╔══██╗╚════██║██║╚════██║   ██║   ██╔══██║██║╚██╗██║██║     ██╔══╝
██║     ███████╗██║  ██║███████║██║███████║   ██║   ██║  ██║██║ ╚████║╚██████╗███████╗
╚═╝     ╚══════╝╚═╝  ╚═╝╚══════╝╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝
*/

void codes_load()
{
#ifdef DEBUG
  Serial.println("read config...");
#endif
  File f = LittleFS.open("/codes.bin", "r");
  if (f)
  {
    IrResult tmp;
    int size = f.size();
    for (int i = 0; i < size; i += sizeof(IrResult))
    {
      f.read((uint8_t *)&tmp, sizeof(IrResult));
#ifdef DEBUG
      Serial.printf("name %s - protocol %x - size %d - value %x\n", tmp.name, tmp.protocol, tmp.size, (int)tmp.value);
      _hexdump((void *)&tmp, sizeof(IrResult));
#endif
      ir_codes.push_back(tmp);
    }
  }
  else
  {
#ifdef DEBUG
    Serial.println("file open failed (read)");
#endif
  }
}

void codes_save()
{
#ifdef DEBUG
  Serial.println("write config...");
#endif
  File f = LittleFS.open("/codes.bin", "w");
  if (f)
  {
    //para cada entrada...
    for (auto i = ir_codes.cbegin(); i != ir_codes.cend(); ++i)
    {
      IrResult tmp;
      memset(&tmp, 0, sizeof(IrResult));

      //copy struct
      strcpy(tmp.name, (*i).name);
      tmp.protocol = (*i).protocol;
      tmp.size = (*i).size;
      tmp.value = (*i).value;
#ifdef DEBUG
      Serial.printf("name %s\tprotocol %x\tsize %d\tvalue %x\n", tmp.name, tmp.protocol, tmp.size, (int)tmp.value);
      _hexdump((void *)&tmp, sizeof(IrResult));
#endif
      f.write((const uint8_t *)&tmp, sizeof(IrResult));
    }
    f.close();
  }
  else
  {
#ifdef DEBUG
    Serial.println("file open failed (write)");
#endif
  }
}

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
<a class='doc' href='/config'>Configurações</a><br>\
</nav>\
</footer>\
</body>\
</html>";

void send_html(const char *h)
{
  char *r = (char *)_malloc(strlen(h) + strlen(html_header) + strlen(html_footer) + 16);
  strcpy(r, html_header);
  strcat(r, h);
  strcat(r, html_footer);
  server.send(200, "text/html", r);
#ifdef DEBUG
  Serial.println(r);
#endif
  free(r);
}

void handle_root()
{
  char *r = (char *)_malloc(96 + (ir_codes.size() * 304) + 16);
  strcpy(r, "<table><tbody><thead><tr><th>Name</th><th>Button</th><th>Delete</th></tr></thead>");
  char n[8];
  int c = 0;
  for (auto i = ir_codes.cbegin(); i != ir_codes.cend(); ++i)
  {
    itoa(c, n, 10);
    strcat(r, "<tr><td data-label='Name'>");
    //name
    strcat(r, (*i).name);
    strcat(r, "</td><td data-label='Button'>");
    //index
    strcat(r, "<input type='button' value='PRESS' onclick='window.location.href=\"/press?button=");
    strcat(r, n);
    strcat(r, "\";'/></td><td data-label='Delete'><input type='button' value='DELETE' onclick='window.location.href=\"/del?button=");
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
  strcat(r, "<br><br><br>\
<form action='/save' method='get'>\
<p>1. Entre o nome do botão que você quer adicionar</p>\
<p>2. Espere a luz do WIFIIR começar a piscar</p>\
<p>3. Aperte o botão no controle até a luz desligar</p>\
<p>4. Clique em ADICIONAR abaixo</p>\
<input type='text' name='button' maxlength='31' value='");
  strcat(r, button.c_str());
  strcat(r, "'>\
<input type='submit' value='Salvar'>\
<br><br><br>\
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
  int button;
  if (server.hasArg("button"))
  {
    button = atoi(server.arg("button").c_str());
    auto i = ir_codes.begin() + button;
    //###irsend.send((*i).protocol, (*i).value, (*i).size);
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
  send_html("<br><br><br>\
<form action='/clear' method='get'>\
Limpar botões salvos:<br>\
<input type='submit' value='Limpar'>\
</form>\
<form action='/reset' method='get'>\
Limpar configurações WIFI:<br>\
<input type='submit' value='Limpar'>\
</form>\
<form method='POST' action='/update' enctype='multipart/form-data'>\
Atualizar Firmware:<br>\
<input type='file' accept='.bin,.bin.gz' name='firmware'>\
<input type='submit' value='Update Firmware'>\
</form>\
<br><br><br>");
}

void handle_clear()
{
  ir_codes.clear();
  LittleFS.remove("/codes.bin");
  send_html("<div class='card warning'>Botões limpos</div><script>setTimeout(function (){document.location.href = '/controle';}, 500);</script>");
}

void handle_reset()
{
  send_html("<p>Reseting...</p>");
  wm.resetSettings();
  delay(1000);
  wm.reboot();
  delay(2000);
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
  server.on("/save", handle_save);
  server.on("/reset", handle_reset);
  server.on("/config", handle_config);
  server.on("/clear", handle_clear);
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

  dump_esp8266();

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
  httpUpdater.setup(&server, "/update");

  install_www_handlers();

  server.begin();
  MDNS.addService("http", "tcp", 80);

  //blinking LED setup
  decoding_onoff = false;
  pinMode(LED_PIN, OUTPUT);
  led_status = LOW;
  digitalWrite(LED_PIN, led_status);

  //carrega botões salvos
  if (!LittleFS.begin())
  {
#ifdef DEBUG
    Serial.println("An Error has occurred while mounting LittleFS");
#endif
  }

  dump_fs();

  codes_load();
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
    if (irrecv.decode(&results))
    {
      //salva codigos
      irresult.protocol = results.decode_type;
      irresult.size = results.bits;
      irresult.value = results.value;

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
}

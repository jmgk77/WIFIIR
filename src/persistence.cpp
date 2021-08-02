/*
██████╗ ███████╗██████╗ ███████╗██╗███████╗████████╗ █████╗ ███╗   ██╗ ██████╗███████╗
██╔══██╗██╔════╝██╔══██╗██╔════╝██║██╔════╝╚══██╔══╝██╔══██╗████╗  ██║██╔════╝██╔════╝
██████╔╝█████╗  ██████╔╝███████╗██║███████╗   ██║   ███████║██╔██╗ ██║██║     █████╗
██╔═══╝ ██╔══╝  ██╔══██╗╚════██║██║╚════██║   ██║   ██╔══██║██║╚██╗██║██║     ██╔══╝
██║     ███████╗██║  ██║███████║██║███████║   ██║   ██║  ██║██║ ╚████║╚██████╗███████╗
╚═╝     ╚══════╝╚═╝  ╚═╝╚══════╝╚═╝╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝
*/

#include "persistence.h"

void codes_load()
{
#ifdef DEBUG
  Serial.println("CODES::LOAD");
#endif
#ifdef SUPPORT_LITTLEFS
  File f = LittleFS.open("/codes.bin", "r");
#else
  File f = SPIFFS.open("/codes.bin", "r");
#endif
  if (f)
  {
    IrResult tmp;
    int size = f.size();
    //pre-aloca codigos
    ir_codes.reserve((size / sizeof(IrResult)));
#ifdef DEBUG
    Serial.printf("(%d)\n", (size / sizeof(IrResult)));
#endif
    for (int i = 0; i < size; i += sizeof(IrResult))
    {
      f.read((uint8_t *)&tmp, sizeof(IrResult));
#ifdef DEBUG_CODES
      Serial.printf(resultToHumanReadableBasic(&tmp.results).c_str());
      _hexdump((void *)&tmp, sizeof(IrResult));
#endif
      ir_codes.push_back(tmp);
    }
  }
  else
  {
#ifdef DEBUG
    Serial.println("file open failed");
#endif
  }
}

void codes_save()
{
#ifdef DEBUG
  Serial.println("CODES::SAVE");
#endif
#ifdef SUPPORT_LITTLEFS
  File f = LittleFS.open("/codes.bin", "w");
#else
  File f = SPIFFS.open("/codes.bin", "w");
#endif
  if (f)
  {
    //para cada entrada...
    for (auto i = ir_codes.cbegin(); i != ir_codes.cend(); ++i)
    {
#ifdef DEBUG_CODES
      Serial.printf(resultToHumanReadableBasic(&(*i).results).c_str());
      _hexdump((void *)(*i).name, sizeof(IrResult));
#endif
      f.write((const uint8_t *)(*i).name, sizeof(IrResult));
    }
    f.close();
  }
  else
  {
#ifdef DEBUG
    Serial.println("file open failed");
#endif
  }
  //reconstroi teclado telegram
#ifdef SUPPORT_TELEGRAM
  tb_kbd();
#endif
}

#ifdef SUPPORT_TELEGRAM
void telegram_load()
{
#ifdef DEBUG
  Serial.println("TOKEN::LOAD");
#endif
  bt_token = "";
#ifdef SUPPORT_LITTLEFS
  File f = LittleFS.open("/token.txt", "r");
#else
  File f = SPIFFS.open("/token.txt", "r");
#endif
  if (f)
  {
    char b[64];
    f.read((uint8_t *)&b, f.size());
    bt_token = String(b);
    f.close();
  }
  else
  {
#ifdef DEBUG
    Serial.println("file open failed");
#endif
  }
}

void telegram_save()
{
#ifdef DEBUG
  Serial.println("TOKEN::SAVE");
#endif
#ifdef SUPPORT_LITTLEFS
  File f = LittleFS.open("/token.txt", "w");
#else
  File f = SPIFFS.open("/token.txt", "w");
#endif
  if (f)
  {
    char b[64];
    strcpy(b, bt_token.c_str());
    f.write(b, strlen(b));
    f.close();
  }
  else
  {
#ifdef DEBUG
    Serial.println("file open failed");
#endif
  }
}

void telegram_users_load()
{
#ifdef DEBUG
  Serial.println("USERS::LOAD");
#endif
#ifdef SUPPORT_LITTLEFS
  File f = LittleFS.open("/users.bin", "r");
#else
  File f = SPIFFS.open("/users.bin", "r");
#endif
  if (f)
  {
    BTUsers tmp;
    int size = f.size();
    //pre-aloca usuarios
    ir_codes.reserve((size / sizeof(BTUsers)));
#ifdef DEBUG
    Serial.printf("(%d)\n", (size / sizeof(BTUsers)));
#endif
    for (int i = 0; i < size; i += sizeof(BTUsers))
    {
      f.read((uint8_t *)&tmp, sizeof(BTUsers));
#ifdef DEBUG
      Serial.printf("BT_LOAD: %d\t%d\t%s\n", tmp.auth, tmp.id, tmp.name);
#endif
      bt_users.push_back(tmp);
    }
  }
  else
  {
#ifdef DEBUG
    Serial.println("file open failed");
#endif
  }
}

void telegram_users_save()
{

#ifdef DEBUG
  Serial.println("USERS::SAVE");
#endif
#ifdef SUPPORT_LITTLEFS
  File f = LittleFS.open("/users.bin", "w");
#else
  File f = SPIFFS.open("/users.bin", "w");
#endif
  if (f)
  {
    //para cada entrada...
    for (auto i = bt_users.cbegin(); i != bt_users.cend(); ++i)
    {
#ifdef DEBUG
      Serial.printf("BT_SAVE: %d\t%d\t%s\n", (*i).auth, (*i).id, (*i).name);
#endif
      if ((*i).auth)
      {
        //so salva usuarios autorizados
        f.write((const uint8_t *)&(*i).auth, sizeof(BTUsers));
      }
    }
    f.close();
  }
  else
  {
#ifdef DEBUG
    Serial.println("file open failed");
#endif
  }
}

#endif

void wifiir_name_load()
{
#ifdef DEBUG
  Serial.println("NAME::LOAD");
#endif
#ifdef SUPPORT_LITTLEFS
  File f = LittleFS.open("/name.txt", "r");
#else
  File f = SPIFFS.open("/name.txt", "r");
#endif
  if (f)
  {
    char tmp[32];
    memset(tmp, 0, sizeof(tmp));
    f.read((uint8_t *)&tmp, sizeof(tmp) - 1);
    wifiir_subname = String(tmp);
#ifdef DEBUG
    Serial.printf("WIFIIR subname: %s\n", (uint8_t *)&tmp);
#endif
  }
  else
  {
#ifdef DEBUG
    Serial.println("file open failed");
#endif
  }
}

void wifiir_name_save()
{
#ifdef DEBUG
  Serial.println("NAME::SAVE");
#endif
#ifdef SUPPORT_LITTLEFS
  File f = LittleFS.open("/name.txt", "w");
#else
  File f = SPIFFS.open("/name.txt", "w");
#endif
  if (f)
  {
    char tmp[8];
    strncpy((char *)&tmp, wifiir_subname.c_str(), sizeof(tmp));
    f.write((char *)&tmp, strlen(tmp));
    f.close();
#ifdef DEBUG
    Serial.printf("WIFIIR subname: %s (%d)\n", (char *)&tmp, strlen(tmp));
#endif
  }
  else
  {
#ifdef DEBUG
    Serial.println("file open failed");
#endif
  }
}

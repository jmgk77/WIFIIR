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
  _Serial.println("CODES::LOAD");
#endif
  File f = LittleFS.open("/codes.bin", "r");

  if (f)
  {
    CODES tmp;
    int size = f.size();
    //pre-aloca codigos
    ir_codes.reserve((size / sizeof(CODES)));
#ifdef DEBUG
    if (size % sizeof(CODES))
    {
      _Serial.println("ERROR: codes.bin dont seens right...");
    }
#endif
#ifdef DEBUG
    _Serial.printf("(%d)\n", (size / sizeof(CODES)));
#endif
    for (int i = 0; i < size; i += sizeof(CODES))
    {
      f.read((uint8_t *)&tmp, sizeof(CODES));
#ifdef DEBUG_CODES
      _Serial.printf(resultToHumanReadableBasic(&tmp.results).c_str());
      _hexdump((void *)&tmp, sizeof(CODES));
#endif
      ir_codes.push_back(tmp);
    }
  }
  else
  {
#ifdef DEBUG
    _Serial.println("file open failed");
#endif
  }
}

void codes_save()
{
#ifdef DEBUG
  _Serial.println("CODES::SAVE");
#endif
  File f = LittleFS.open("/codes.bin", "w");

  if (f)
  {
    //para cada entrada...
    for (auto i = ir_codes.cbegin(); i != ir_codes.cend(); ++i)
    {
#ifdef DEBUG_CODES
      _Serial.printf(resultToHumanReadableBasic(&i->results).c_str());
      _hexdump((void *)i->name, sizeof(CODES));
#endif
      f.write(&i->_start, sizeof(CODES));
    }
    f.close();
  }
  else
  {
#ifdef DEBUG
    _Serial.println("file open failed");
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
  _Serial.println("TOKEN::LOAD");
#endif
  bt_token = "";
  File f = LittleFS.open("/token.txt", "r");

  if (f)
  {
    char b[64];
    memset(&b, 0, sizeof(b));
    f.read((uint8_t *)b, f.size());
    bt_token = String(b);
    f.close();
  }
  else
  {
#ifdef DEBUG
    _Serial.println("file open failed");
#endif
  }
}

void telegram_save()
{
#ifdef DEBUG
  _Serial.println("TOKEN::SAVE");
#endif
  File f = LittleFS.open("/token.txt", "w");

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
    _Serial.println("file open failed");
#endif
  }
}

void telegram_users_load()
{
#ifdef DEBUG
  _Serial.println("TUSERS::LOAD");
#endif
  File f = LittleFS.open("/users.bin", "r");

  if (f)
  {
    TUSERS tmp;
    int size = f.size();
    //pre-aloca usuarios
    ir_codes.reserve((size / sizeof(TUSERS)));
#ifdef DEBUG
    if (size % sizeof(TUSERS))
    {
      _Serial.println("ERROR: users.bin dont seens right...");
    }
#endif
#ifdef DEBUG
    _Serial.printf("(%d)\n", (size / sizeof(TUSERS)));
#endif
    for (int i = 0; i < size; i += sizeof(TUSERS))
    {
      f.read((uint8_t *)&tmp, sizeof(TUSERS));
#ifdef DEBUG
      _Serial.printf("BT_LOAD: %d\t%d\t%s\n", tmp.auth, tmp.id, tmp.name);
#endif
      bt_users.push_back(tmp);
    }
  }
  else
  {
#ifdef DEBUG
    _Serial.println("file open failed");
#endif
  }
}

void telegram_users_save()
{

#ifdef DEBUG
  _Serial.println("TUSERS::SAVE");
#endif
  File f = LittleFS.open("/users.bin", "w");

  if (f)
  {
    //para cada entrada...
    for (auto i = bt_users.cbegin(); i != bt_users.cend(); ++i)
    {
#ifdef DEBUG
      _Serial.printf("BT_SAVE: %d\t%d\t%s\n", i->auth, i->id, i->name);
#endif
      if (i->auth)
      {
        //so salva usuarios autorizados
        f.write(&i->_start, sizeof(TUSERS));
      }
    }
    f.close();
  }
  else
  {
#ifdef DEBUG
    _Serial.println("file open failed");
#endif
  }
}

#endif

void wifiir_name_load()
{
#ifdef DEBUG
  _Serial.println("NAME::LOAD");
#endif
  File f = LittleFS.open("/name.txt", "r");

  if (f)
  {
    char tmp[32];
    memset(tmp, 0, sizeof(tmp));
    f.read((uint8_t *)&tmp, sizeof(tmp) - 1);
    wifiir_subname = String(tmp);
#ifdef DEBUG
    _Serial.printf("Subname: %s\n", (uint8_t *)&tmp);
#endif
  }
  else
  {
#ifdef DEBUG
    _Serial.println("file open failed");
#endif
  }
}

void wifiir_name_save()
{
#ifdef DEBUG
  _Serial.println("NAME::SAVE");
#endif
  File f = LittleFS.open("/name.txt", "w");

  if (f)
  {
    char tmp[32];
    strncpy((char *)&tmp, wifiir_subname.c_str(), sizeof(tmp));
    f.write(tmp, strlen(tmp));
    f.close();
#ifdef DEBUG
    _Serial.printf("Subname: %s (%d)\n", (char *)&tmp, strlen(tmp));
#endif
  }
  else
  {
#ifdef DEBUG
    _Serial.println("file open failed");
#endif
  }
}

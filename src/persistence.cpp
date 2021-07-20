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
      Serial.printf(resultToHumanReadableBasic(&tmp.results).c_str());
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
#ifdef DEBUG
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
    Serial.println("file open failed (write)");
#endif
  }
}

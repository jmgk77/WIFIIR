/*
██████╗ ███████╗██████╗ ██╗   ██╗ ██████╗
██╔══██╗██╔════╝██╔══██╗██║   ██║██╔════╝
██║  ██║█████╗  ██████╔╝██║   ██║██║  ███╗
██║  ██║██╔══╝  ██╔══██╗██║   ██║██║   ██║
██████╔╝███████╗██████╔╝╚██████╔╝╚██████╔╝
╚═════╝ ╚══════╝╚═════╝  ╚═════╝  ╚═════╝
*/

#include "_debug.h"

#ifdef DEBUG
void dump_ir(CODES irresult)
{
    _Serial.println("IR CODE");
    String s;
    switch (irresult.results.decode_type)
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
    _Serial.print("PROTOCOL: ");
    _Serial.println(s);
    _Serial.print("VALUE: ");
    _Serial.println(irresult.results.value);
    _Serial.print("SIZE: ");
    _Serial.println(irresult.results.bits);
}

void dump_rf(CODES irresult)
{
    _Serial.println("RF CODE");
    _Serial.print("CODE: ");
    _Serial.println(irresult.rfcode.code);
    _Serial.print("LENGHT: ");
    _Serial.println(irresult.rfcode.length);
    _Serial.print("DELAY: ");
    _Serial.println(irresult.rfcode.delay);
    _Serial.print("PROTOCOL: ");
    _Serial.println(irresult.rfcode.protocol);
}

void _hexdump(void *ptr, int buflen)
{
    unsigned char *buf = (unsigned char *)ptr;
    int i, j;
    char s[128];
    for (i = 0; i < buflen; i += 16)
    {
        sprintf(s, "%06x: ", i);
        _Serial.print(s);
        for (j = 0; j < 16; j++)
            if (i + j < buflen)
            {
                sprintf(s, "%02x ", buf[i + j]);
                _Serial.print(s);
            }
            else
            {
                sprintf(s, "   ");
                _Serial.print(s);
            }
        sprintf(s, " ");
        _Serial.print(s);
        for (j = 0; j < 16; j++)
            if (i + j < buflen)
            {
                sprintf(s, "%c", isprint(buf[i + j]) ? buf[i + j] : '.');
                _Serial.print(s);
            }
        sprintf(s, "\n");
        _Serial.print(s);
    }
}

void dump_fs()
{
    FSInfo fs_info;
    LittleFS.info(fs_info);
    _Serial.printf("FS_INFO\ntotalBytes: %d\nusedBytes: %d\nblockSize: %d\npageSize: %d\nmaxOpenFiles: %d\nmaxPathLength: %d\n",
                   fs_info.totalBytes, fs_info.usedBytes, fs_info.blockSize, fs_info.pageSize, fs_info.maxOpenFiles, fs_info.maxPathLength);
}

void dump_esp8266()
{
    _Serial.printf("ESP8266_INFO\nESP.getBootMode(): %d\nESP.getSdkVersion(): %s\nESP.getBootVersion(): %d\nESP.getChipId(): %08x\n\
    ESP.getFlashChipSize(): %d\nESP.getFlashChipRealSize(): %d\nESP.getFlashChipSizeByChipId(): %d\nESP.getFlashChipId(): %08x\n\
    ESP.getFreeHeap(): %d\n",
                   ESP.getBootMode(), ESP.getSdkVersion(), ESP.getBootVersion(), ESP.getChipId(), ESP.getFlashChipSize(),
                   ESP.getFlashChipRealSize(), ESP.getFlashChipSizeByChipId(), ESP.getFlashChipId(), ESP.getFreeHeap());
}

#ifdef DEBUG_MALLOC
void *_malloc(size_t size)
{
    uint32_t free;
    uint16_t max;
    uint8_t frag;
    ESP.getHeapStats(&free, &max, &frag);
    _Serial.printf("MALLOC: asked %d (free: %5d - max: %5d - frag: %3d%%)\n", size, free, max, frag);
    void *m = malloc(size);
    if (!m)
    {
        _Serial.println("MALLOC ERROR!");
    }
    return m;
}
#endif

#ifdef DEBUG_SERIAL2FILE
Serial2File _Serial;
#endif

#endif

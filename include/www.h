/*
██╗    ██╗██╗    ██╗██╗    ██╗    ██╗  ██╗ █████╗ ███╗   ██╗██████╗ ██╗     ███████╗██████╗ ███████╗
██║    ██║██║    ██║██║    ██║    ██║  ██║██╔══██╗████╗  ██║██╔══██╗██║     ██╔════╝██╔══██╗██╔════╝
██║ █╗ ██║██║ █╗ ██║██║ █╗ ██║    ███████║███████║██╔██╗ ██║██║  ██║██║     █████╗  ██████╔╝███████╗
██║███╗██║██║███╗██║██║███╗██║    ██╔══██║██╔══██║██║╚██╗██║██║  ██║██║     ██╔══╝  ██╔══██╗╚════██║
╚███╔███╔╝╚███╔███╔╝╚███╔███╔╝    ██║  ██║██║  ██║██║ ╚████║██████╔╝███████╗███████╗██║  ██║███████║
 ╚══╝╚══╝  ╚══╝╚══╝  ╚══╝╚══╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝
*/

#pragma once

#include "main.h"

extern ESP8266WebServer server;
extern CODES irresult;
extern IRsend irsend;
extern RCSwitch rf;
extern WiFiManager wm;

extern bool decoding_onoff;
extern bool waiting_ir_rf;
extern void _end_ir();

extern std::vector<CODES> ir_codes;

extern String boot_time;

#ifdef SUPPORT_TELEGRAM
extern String bt_token;
extern std::vector<TUSERS> bt_users;
#endif

void install_www_handlers();

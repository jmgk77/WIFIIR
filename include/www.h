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
extern ESP8266WebServer server;
extern bool decoding_onoff;
extern IrResult irresult;
extern IRsend irsend;
extern WiFiManager wm;

extern std::vector<IrResult> ir_codes;

#ifdef SUPPORT_TELEGRAM
extern String bt_token;
extern std::vector<BTUsers> bt_users;
#endif

void install_www_handlers();

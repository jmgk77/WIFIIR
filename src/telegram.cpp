/*
████████╗███████╗██╗     ███████╗ ██████╗ ██████╗  █████╗ ███╗   ███╗
╚══██╔══╝██╔════╝██║     ██╔════╝██╔════╝ ██╔══██╗██╔══██╗████╗ ████║
   ██║   █████╗  ██║     █████╗  ██║  ███╗██████╔╝███████║██╔████╔██║
   ██║   ██╔══╝  ██║     ██╔══╝  ██║   ██║██╔══██╗██╔══██║██║╚██╔╝██║
   ██║   ███████╗███████╗███████╗╚██████╔╝██║  ██║██║  ██║██║ ╚═╝ ██║
   ╚═╝   ╚══════╝╚══════╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝
*/

#include "telegram.h"

String bt_token;

CTBot tb;

void bt_setup()
{
    if (!bt_token.isEmpty())
    {
        tb.setTelegramToken(bt_token);
        //tb.useDNS(true);
        //tb.setFingerprint(0);
#ifdef DEBUG
        Serial.print("BOT ");
        if (tb.testConnection())
        {
            Serial.println("OK");
        }
        else
        {
            Serial.println("NOK");
        }
#endif
    }
    else
    {
#ifdef DEBUG
        Serial.println("NO BOT");
#endif
    }
}

void bt_loop()
{
    TBMessage msg;
    if (tb.getNewMessage(msg) != CTBotMessageNoData)
    {
        //###
    }
}

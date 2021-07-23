/*
████████╗███████╗██╗     ███████╗ ██████╗ ██████╗  █████╗ ███╗   ███╗
╚══██╔══╝██╔════╝██║     ██╔════╝██╔════╝ ██╔══██╗██╔══██╗████╗ ████║
   ██║   █████╗  ██║     █████╗  ██║  ███╗██████╔╝███████║██╔████╔██║
   ██║   ██╔══╝  ██║     ██╔══╝  ██║   ██║██╔══██╗██╔══██║██║╚██╔╝██║
   ██║   ███████╗███████╗███████╗╚██████╔╝██║  ██║██║  ██║██║ ╚═╝ ██║
   ╚═╝   ╚══════╝╚══════╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚═╝
*/

#include "telegram.h"

#ifdef SUPPORT_TELEGRAM

String bt_token;
CTBotInlineKeyboard tkbd;
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

void tb_kbd()
{
    if (!bt_token.isEmpty())
    {
        tkbd.flushData();
        char n[8];
        int c = 0;
        for (auto i = ir_codes.cbegin(); i != ir_codes.cend(); ++i)
        {
            itoa(c, n, 10);
            tkbd.addButton((*i).name, n, CTBotKeyboardButtonQuery);
#ifdef ITENS_PER_ROW
            if (c % ITENS_PER_ROW == (ITENS_PER_ROW - 1))
            {
                tkbd.addRow();
            }
#endif
            c++;
        }
#ifdef DEBUG
        Serial.println(tkbd.getJSON());
#endif
    }
}

void bt_loop()
{
    if (!bt_token.isEmpty())
    {
        TBMessage msg;
        if (tb.getNewMessage(msg) != CTBotMessageNoData)
        {
#ifdef DEBUG
            Serial.print("MSG");
#endif
            if (msg.messageType == CTBotMessageQuery)
            {
#ifdef DEBUG
                Serial.print(":CALLBACK");
#endif
                int c = msg.callbackQueryData.toInt();
#ifdef DEBUG
                Serial.print(":BUTTON:");
                Serial.println(c);
#endif
                ir_send(c);
                tb.endQuery(msg.callbackQueryID, "Botão enviado!");
            }
            else
            {
#ifdef DEBUG
                Serial.println(":OTHER");
#endif
                tb.sendMessage(msg.sender.id, "Escolha...", tkbd);
            }
        }
    }
}

#endif

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

std::vector<BTUsers> bt_users;

void bt_setup()
{
    if (!bt_token.isEmpty())
    {
        tb.setTelegramToken(bt_token);
        //tb.useDNS(true);
        //tb.setFingerprint(0);
#ifdef DEBUG
        _Serial.print("BOT ");
        if (tb.testConnection())
        {
            _Serial.println("OK");
        }
        else
        {
            _Serial.println("NOK");
        }
#endif
    }
    else
    {
#ifdef DEBUG
        _Serial.println("NO BOT");
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
    }
}

void bt_adduser(int id, const char *username)
{
    BTUsers tmp;
    tmp.auth = false;
    tmp.id = id;
    strncpy(tmp.name, username, sizeof(tmp.name));
    bt_users.push_back(tmp);
#ifdef DEBUG
    _Serial.printf("BT_ADD: %d\t%d\t%s\n", tmp.auth, tmp.id, tmp.name);
#endif
}

bool bt_check(int id)
{
    for (auto i = bt_users.cbegin(); i != bt_users.cend(); ++i)
    {
#ifdef DEBUG
        _Serial.printf("BT_CHECK: %d\t%d\t%s\n", (*i).auth, (*i).id, (*i).name);
#endif
        if ((*i).id == id)
        {
            return (*i).auth;
        }
    }
    return false;
}

void bt_loop()
{
    if (!bt_token.isEmpty())
    {
        TBMessage msg;
        if (tb.getNewMessage(msg) != CTBotMessageNoData)
        {

            if (bt_check(msg.sender.id))
            {
#ifdef DEBUG
                _Serial.print("MSG");
#endif
                if (msg.messageType == CTBotMessageQuery)
                {
#ifdef DEBUG
                    _Serial.print(":CALLBACK");
#endif
                    int c = msg.callbackQueryData.toInt();
#ifdef DEBUG
                    _Serial.print(":BUTTON:");
                    _Serial.println(c);
#endif
                    ir_send(c);
                    tb.endQuery(msg.callbackQueryID, "Botão enviado!");
                }
                else
                {
#ifdef DEBUG
                    _Serial.println(":OTHER");
#endif
                    tb.sendMessage(msg.sender.id, "Escolha...", tkbd);
                }
            }
            else
            {
                bt_adduser(msg.sender.id, msg.sender.firstName.c_str());
                tb.sendMessage(msg.sender.id, "Não autorizado (acesse a interface WWW para conceder acesso)");
            }
        }
    }
}

#endif

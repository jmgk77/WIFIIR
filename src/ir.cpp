/*
██╗██████╗
██║██╔══██╗
██║██████╔╝
██║██╔══██╗
██║██║  ██║
╚═╝╚═╝  ╚═╝
*/

#include "ir.h"

void ir_send(int button)
{
        auto i = ir_codes.begin() + button;
        //send
        if ((*i).results.decode_type == decode_type_t::UNKNOWN)
        {
#ifdef DEBUG
                Serial.println("SEND RAW");
#endif
                uint16_t *raw_array = resultToRawArray(&(*i).results);
                irsend.sendRaw(raw_array, getCorrectedRawLength(&(*i).results), 38000);
                delete[] raw_array;
        }
        else if (hasACState((*i).results.decode_type))
        {
#ifdef DEBUG
                Serial.println("SEND AC");
#endif
                irsend.send((*i).results.decode_type, (*i).results.state, (*i).results.bits / 8);
        }
        else
        {
#ifdef DEBUG
                Serial.println("SEND NORMAL");
#endif
                irsend.send((*i).results.decode_type, (*i).results.value, (*i).results.bits);
        }
}

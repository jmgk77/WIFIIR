/*
██████╗ ███████╗██████╗ ██╗   ██╗ ██████╗
██╔══██╗██╔════╝██╔══██╗██║   ██║██╔════╝
██║  ██║█████╗  ██████╔╝██║   ██║██║  ███╗
██║  ██║██╔══╝  ██╔══██╗██║   ██║██║   ██║
██████╔╝███████╗██████╔╝╚██████╔╝╚██████╔╝
╚═════╝ ╚══════╝╚═════╝  ╚═════╝  ╚═════╝
*/

#pragma once

#include "main.h"

#ifdef DEBUG
void dump_ir(CODES irresult);
void dump_rf(CODES irresult);
void _hexdump(void *ptr, int buflen);
void dump_fs();
void dump_esp8266();
void *_malloc(size_t size);
#endif

#ifdef DEBUG_SERIAL2FILE
class Serial2File
{
public:
    void begin(int x)
    {
        Serial.begin(x);
    }
    void setDebugOutput(bool x)
    {
        Serial.setDebugOutput(x);
    }
    void print(uint64_t x)
    {
        _start();
        f.print(x);
        _end();
        Serial.print(x);
    }
    void print(const char *t)
    {
        _start();
        f.print(t);
        _end();
        Serial.print(t);
    }
    void print(String s)
    {
        _start();
        f.print(s);
        _end();
        Serial.print(s);
    }
    void println(uint64_t x)
    {
        _start();
        f.println(x);
        _end();
        Serial.println(x);
    }
    void println(const char *t)
    {
        _start();
        f.println(t);
        _end();
        Serial.println(t);
    }
    void println(String s)
    {
        _start();
        f.println(s);
        _end();
        Serial.println(s);
    }
    void println()
    {
        _start();
        f.println("");
        _end();
        Serial.println("");
    }
    void printf(const char *format, ...)
    {
        char buf[1024];
        va_list myargs;
        va_start(myargs, format);
        vsnprintf((char *)buf, sizeof(buf), format, myargs);
        print(buf);
        va_end(myargs);
    }

private:
    File f;
    void _start()
    {
        f = LittleFS.open("/log.txt", "a");
    }
    void _end()
    {
        f.close();
    }
};

extern Serial2File _Serial;

#endif

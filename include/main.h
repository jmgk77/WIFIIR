//██╗    ██╗██╗███████╗██╗██╗██████╗
//██║    ██║██║██╔════╝██║██║██╔══██╗
//██║ █╗ ██║██║█████╗  ██║██║██████╔╝
//██║███╗██║██║██╔══╝  ██║██║██╔══██╗
//╚███╔███╔╝██║██║     ██║██║██║  ██║
// ╚══╝╚══╝ ╚═╝╚═╝     ╚═╝╚═╝╚═╝  ╚═╝

#pragma once

#define DEBUG 1

#ifndef DEBUG
#define _malloc malloc
#endif

#include <Arduino.h>

#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

#include <LittleFS.h>

#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

typedef struct
{
    char name[32];
    decode_results results;
} IrResult;

#include "_debug.h"
#include "persistence.h"
#include "www.h"

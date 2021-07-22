/*
██╗    ██╗██╗███████╗██╗██╗██████╗
██║    ██║██║██╔════╝██║██║██╔══██╗
██║ █╗ ██║██║█████╗  ██║██║██████╔╝
██║███╗██║██║██╔══╝  ██║██║██╔══██╗
╚███╔███╔╝██║██║     ██║██║██║  ██║
 ╚══╝╚══╝ ╚═╝╚═╝     ╚═╝╚═╝╚═╝  ╚═╝
*/

#pragma once

#define DEBUG
//#define DEBUG_SEND
//#define DEBUG_MALLOC

#ifndef DEBUG_MALLOC
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

#include <CTBot.h>

typedef struct
{
    char name[32];
    decode_results results;
} IrResult;

#include "_debug.h"
#include "persistence.h"
#include "www.h"
#include "telegram.h"
#include "ir.h"

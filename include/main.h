/*
██╗    ██╗██╗███████╗██╗██╗██████╗
██║    ██║██║██╔════╝██║██║██╔══██╗
██║ █╗ ██║██║█████╗  ██║██║██████╔╝
██║███╗██║██║██╔══╝  ██║██║██╔══██╗
╚███╔███╔╝██║██║     ██║██║██║  ██║
 ╚══╝╚══╝ ╚═╝╚═╝     ╚═╝╚═╝╚═╝  ╚═╝
*/

#pragma once

//#define DEBUG
//#define DEBUG_SEND
//#define DEBUG_MALLOC
//#define DEBUG_CODES
//#define DEBUG_GENRND 22

#define SUPPORT_LITTLEFS
#define SUPPORT_OTA
#define SUPPORT_TELEGRAM

#ifndef DEBUG_MALLOC
#define _malloc malloc
#endif

#include <Arduino.h>

#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

#ifdef SUPPORT_LITTLEFS
#include <LittleFS.h>
#else
#include "FS.h"
#endif

#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

#ifdef SUPPORT_TELEGRAM
#include <CTBot.h>
#endif

typedef struct
{
    char name[32];
    decode_results results;
} IrResult;

#include "_debug.h"
#include "persistence.h"
#include "www.h"
#ifdef SUPPORT_TELEGRAM
#include "telegram.h"
#endif
#include "ir.h"

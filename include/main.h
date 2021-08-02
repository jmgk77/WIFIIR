/*
██╗    ██╗██╗███████╗██╗██╗██████╗
██║    ██║██║██╔════╝██║██║██╔══██╗
██║ █╗ ██║██║█████╗  ██║██║██████╔╝
██║███╗██║██║██╔══╝  ██║██║██╔══██╗
╚███╔███╔╝██║██║     ██║██║██║  ██║
 ╚══╝╚══╝ ╚═╝╚═╝     ╚═╝╚═╝╚═╝  ╚═╝
*/

#pragma once

#define LED_PIN D5
#define IR_LED_PIN D2
#define IR_RECV_PIN D7

#define DEBUG
//#define DEBUG_SEND
//#define DEBUG_MALLOC
//#define DEBUG_F
//#define DEBUG_CODES
//#define DEBUG_GENRNDBTN 4
//#define DEBUG_GENRNDUSR 8
//#define DEBUG_ESP
#define DEBUG_FS

#define SUPPORT_LITTLEFS
#define SUPPORT_OTA
#define SUPPORT_TELEGRAM

#define SUPPORT_MDNS
//#define SUPPORT_NETBIOS
#define SUPPORT_LLMNR
#define SUPPORT_SSDP

#ifndef DEBUG_MALLOC
#define _malloc malloc
#endif

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#ifdef SUPPORT_MDNS
#include <ESP8266mDNS.h>
#endif
#ifdef SUPPORT_NETBIOS
#include <ESP8266NetBIOS.h>
#endif
#ifdef SUPPORT_LLMNR
#include <ESP8266LLMNR.h>
#endif
#ifdef SUPPORT_SSDP
#include <SSDP_esp8266.h>
#endif

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

#ifdef SUPPORT_TELEGRAM
typedef struct
{
    bool auth;
    int id;
    char name[32];
} BTUsers;
#endif

#include "_debug.h"
#include "persistence.h"
#include "www.h"
#ifdef SUPPORT_TELEGRAM
#include "telegram.h"
#endif
#include "ir.h"

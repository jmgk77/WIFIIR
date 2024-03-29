/*
██╗    ██╗██╗███████╗██╗██╗██████╗
██║    ██║██║██╔════╝██║██║██╔══██╗
██║ █╗ ██║██║█████╗  ██║██║██████╔╝
██║███╗██║██║██╔══╝  ██║██║██╔══██╗
╚███╔███╔╝██║██║     ██║██║██║  ██║
 ╚══╝╚══╝ ╚═╝╚═╝     ╚═╝╚═╝╚═╝  ╚═╝
*/

#pragma once

#if !defined(ESP8266)
#error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#define LED_PIN D5
#define IR_LED_PIN D2
#define IR_RECV_PIN D7

#define RF_RX D4
#define RF_TX D6

// #define HW_TEST

#define DEBUG
// #define DEBUG_SEND
// #define DEBUG_MALLOC
// #define DEBUG_F
// #define DEBUG_CODES
// #define DEBUG_GENRNDBTN 4
// #define DEBUG_GENRNDUSR 8
// #define DEBUG_ESP
// #define DEBUG_FS
// #define DEBUG_SERIAL2FILE

#define SUPPORT_OTA
#define SUPPORT_TELEGRAM
#define SUPPORT_IMPORT
#define SUPPORT_MDNS
// #define SUPPORT_NETBIOS
#define SUPPORT_LLMNR
#define SUPPORT_SSDP

//
#ifdef SUPPORT_IMPORT
#define DEBUG_FS
#endif

#ifdef HW_TEST
#define DEBUG
#endif

#ifndef DEBUG_MALLOC
#define _malloc malloc
#endif

#ifndef DEBUG_SERIAL2FILE
#define _Serial Serial
#endif

#include <Arduino.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#ifdef SUPPORT_IMPORT
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#endif

#if defined(SUPPORT_MDNS) || defined(SUPPORT_IMPORT)
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

#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>
#include <LittleFS.h>
#include <RCSwitch.h>

#ifdef SUPPORT_TELEGRAM
#include <CTBot.h>
#endif

#define TIMER_INTERRUPT_DEBUG 0
#define _TIMERINTERRUPT_LOGLEVEL_ 0
#include <ESP8266TimerInterrupt.h>

typedef enum {
  UNUSED_CODE,
  IR_CODE,
  RF_CODE,
} CODES_TYPE;

typedef struct {
  unsigned long code;
  unsigned int length;
  unsigned int delay;
  unsigned int protocol;
} rf_results;

typedef struct {
  uint8_t _start;
  CODES_TYPE type;
  char name[32];
  union {
    decode_results results;
    rf_results rfcode;
  };
} CODES;

#ifdef SUPPORT_TELEGRAM
typedef struct {
  uint8_t _start;
  bool auth;
  int id;
  char name[32];
} TUSERS;
#endif

#include "_debug.h"
#include "persistence.h"
#include "version.h"
#include "www.h"
#ifdef SUPPORT_TELEGRAM
#include "telegram.h"
#endif
#include "ir_rf.h"

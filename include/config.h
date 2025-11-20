#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h> 

const int LED_R = 2;
const int LED_B = 3;
const int LED_G = 4;

const int pin0 = 5;
const int pin1 = 6;
const int pin2 = 10;
const int pin3 = 9;
const float TRIM1 = 0.935f;
const float TRIM2 = 1.0f;

const int PHOTOTRANSISTOR_Visible = A5;
const int LED_IR = 7;
const int PHOTOTRANSISTOR_IR = A4;

const int COLOR_THRESHOLD = 50;
const int DISTANCECALFACTOR = 1;

static const char* SERVERADDRESS = "34.28.153.91";
const int PORT = 80;
static const char* CLIENTID = "DCF2BCAB6F0B";
static const char* MACCHEESEFILTER = "WebClient_DCF2BCAB6F0B.";
static const char* EXTERNALFILTER = "WebClient_DCF2BCAB6F0B.EXT";

static const char* SSID = "tufts_eecs";
static const char* PASS = "foundedin1883";


#endif
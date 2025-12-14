#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h> 

const int BUTTON_PIN = 8;
const int VBAT_PIN = A0;
const float VBAT_SCALE = 2; 

const int INDC_R_PIN = 13;
const int INDC_G_PIN = 12;
const int INDC_B_PIN = 11;

const int LED_R = 2;
const int LED_B = 3;
const int LED_G = 4;

const int pin0 = 6;
const int pin1 = 5;
const int pin2 = 10;
const int pin3 = 9;
const float TRIM1 = 0.97f;
const float TRIM2 = 1.0f;

const int PHOTOTRANSISTOR_Visible = A5;
const int LED_IR = 7;
const int PHOTOTRANSISTOR_IR = A4;

const int COLOR_THRESHOLD = 50;
const int DISTANCECALFACTOR = 1;

//static const char* SERVERADDRESS = "35.239.140.61";
static const char* SERVERADDRESS = "10.5.15.112";
const int PORT = 8080;
static const char* CLIENTID = "DCF2BCAB6F0B";
static const char* MACCHEESEFILTER = "WebClient_DCF2BCAB6F0B";
static const char* EXTERNALFILTER = "F392FC86D8D7";

static const char* SSID = "tufts_eecs";
static const char* PASS = "foundedin1883";


#endif
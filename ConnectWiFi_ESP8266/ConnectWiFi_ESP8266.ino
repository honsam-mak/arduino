/**
 * This example is to initialze the ESP8266 and connect to WIFI
 */
#include "ESP8266.h"
#include <SoftwareSerial.h>

#define SSID        "mark"
//#define SSID        "SS_AP"
#define PASSWORD    "6266790021"

SoftwareSerial esp8266(10, 11);  //RX:10, TX:11
ESP8266 wifi(esp8266);

#define DEBUG true

void setup(void)
{
    Serial.begin(9600);
    // Configure the initial speed of ESP8266
    esp8266.begin(115200);
    
 //   Serial.println("setup begin");

  /*
    if(wifi.kick()) {
      Serial.println("AT OK");
    } else {
      Serial.println("AT Failed");
    }

    Serial.print("FW Version: ");
    Serial.println(wifi.getVersion().c_str());

    if (wifi.setOprToStation()) {
        Serial.print("to station ok\r\n");
    } else {
        Serial.print("to station err\r\n");
    }

    if (wifi.joinAP(SSID, PASSWORD)) {
        Serial.print("Join AP success\r\n");
        Serial.print("IP: ");       
        Serial.println(wifi.getLocalIP().c_str());
    } else {
        Serial.print("Join AP failure\r\n");
    }
    
    Serial.println("setup end"); 
    */
}

void loop(void)
{
  if (esp8266.available()) {
    Serial.write(esp8266.read());
  }
 
  if (Serial.available()) {
    char chars = Serial.read();
    esp8266.write(chars);
  }
}

 

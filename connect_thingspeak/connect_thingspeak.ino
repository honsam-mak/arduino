/*
 * This example is to connect thingspeak.com
 */


#include "ESP8266.h"
#include <SoftwareSerial.h>

#define SSID        "mark"
#define PASSWORD    "6266790021"

#define TS_IP       "184.106.153.149" // api.thingspeak.com
#define WRITE_API_KEY   "6RZFFXN81EIFD1XK"

#define BAUD_MONITOR  9600
#define BAUD_ESP8266  115200
  
SoftwareSerial esp8266(10, 11);  //RX:10, TX:11
ESP8266 wifi(esp8266);

// ThingSpeak Settings
String writeAPIKey = "6RZFFXN81EIFD1XK";

int testValue = 0;

void setup(void)
{
    Serial.begin(BAUD_MONITOR);
    // Configure the initial speed of ESP8266
    esp8266.begin(BAUD_ESP8266);
    
    Serial.println("setup begin");
    connectWiFi();
    Serial.println("setup end"); 
    
}

void loop(void)
{
  // TCP connection
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += TS_IP;
  cmd += "\",80";
  esp8266.println(cmd);

  if(esp8266.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }

  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += WRITE_API_KEY;
  getStr +="&field1=";
  getStr += String(++testValue);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  esp8266.println(cmd);

  if(esp8266.find(">")){
    esp8266.print(getStr);
  }
  else{
    esp8266.println("AT+CIPCLOSE");
    // alert user
    Serial.println("AT+CIPCLOSE");
  }

  // thingspeak needs 15 sec delay between updates
  delay(16000);  
}

void connectWiFi(void) {
    boolean atOK = false;
    do {
      atOK = wifi.kick();
      if(atOK) {
        Serial.println("AT OK");
      } else {
        Serial.println("AT Failed");
        delay(5000);
      }
    }
    while(!atOK);

//    Serial.print("FW Version: ");
//    Serial.println(wifi.getVersion().c_str());

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
}

void typeCommand(void) {
    if (esp8266.available()) {
    Serial.write(esp8266.read());
  }
 
  if (Serial.available()) {
    char chars = Serial.read();
    esp8266.write(chars);
  }
}

void GetFromCloud(void) {}


/*
 * The project will combine all modules, such as LCD, RTC, Temperature,
 * Pressure and WIFI module, and upload the data onto Cloud.
 */

#include "Arduino.h"
#include <DS1302.h>
#include <LiquidCrystal_I2C.h>
#include "ESP8266.h"
#include <SoftwareSerial.h>

#define DELAY_DURATION 1000

#define SSID        "mark"
#define PASSWORD    "6266790021"

#define TS_IP       "184.106.153.149" // api.thingspeak.com
#define WRITE_API_KEY   "6RZFFXN81EIFD1XK"

#define BAUD_MONITOR  9600
#define BAUD_ESP8266  115200

/*
 * RTC Module :
 *     Initialize DS1302.
 *     DS1302 rtc([RST], [DAT], [CLOCK]); 3-pin connection
 */
DS1302 rtc(5, 6, 7);

/*
 * LCD Module :
 *     Set the pins on the I2C chip used for LCD connections:
 */
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

/*
 * Temperature Module
 */
const int sensorPin=A0;

/*
 * Wifi Module
 */
SoftwareSerial esp8266(10, 11);  //RX:10, TX:11
ESP8266 wifi(esp8266);

// ThingSpeak Settings
String writeAPIKey = "6RZFFXN81EIFD1XK";

/*
 * variables
 */
char line1Str[16];   //for RTC Time
char line2Str[16];   //for temperature
char tempStr[16];    //temperature

void rtcModule_setup(bool needTimeSetup) {

  // Setup the clock as normal mode
  rtc.halt(false);

  if(needTimeSetup) {
    // Cancel write-protect, need this line for date setting
    rtc.writeProtect(false);

    // The way to set Time. Just do once before the battery used up
    rtc.setDOW(FRIDAY);        // Week day，e.g. FRIDAY
    rtc.setTime(23, 56, 0);     // Time hr，min，sec (24hr format)
    rtc.setDate(25, 11, 2016);   // Date day，month，year
  }
}

void lcdModule_setup() {

  // initialze LCD, 16 char in one line, total 2 lines, default to turn on the backlight
  lcd.begin(16, 2);

  // blink three times
  for(int i=0; i<3; i++) {
    lcd.backlight();    //turn on backlight
      delay(250);
      lcd.noBacklight();  // turn off backlight
      delay(250);
  }
  lcd.backlight();

  // intial output
  lcd.setCursor(0,0);         //set cursor position at first line
  lcd.print("Project Jonietlo");
  delay(1000);
  lcd.setCursor(0, 1);        //set cursor position at second line
  lcd.print("LCD is ready");
  delay(1000);
}

void tempModule_setup() {;}

void connectWiFi(void) {
    boolean atOK = false;
    int retry = 0;
    do {
      atOK = wifi.kick();
      if(atOK) {
        Serial.println("AT OK");
      } else {
        Serial.println("AT Failed");
        delay(5000);
        retry++;
      }
    } while(!atOK && retry <5);

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

void wifi_setup(void)
{
    Serial.begin(BAUD_MONITOR);
    // Configure the initial speed of ESP8266
    esp8266.begin(BAUD_ESP8266);

    Serial.println("setup begin");
    connectWiFi();
    Serial.println("setup end");

}


void rtcModule_loop() {

  sprintf(line1Str, "Time: %s", rtc.getTimeStr());

}

void tempModule_loop() {

  int sensorVal = analogRead(sensorPin);

  // convert the ADC reading to voltage
  float voltage = (sensorVal/1024.0) * 5.0;

  // convert the voltage to temperature in degrees C
  // the sensor reports 10 mv per degree
  // to degree (voltage times 100)
  float temperature = voltage *100;

  char temp[16];
  dtostrf(temperature, 5, 2, temp);
  sprintf(line2Str, "T: %sC", temp);
    sprintf(tempStr, "%s", temp);
}

void lcdModule_loop() {
  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(line1Str);

  lcd.setCursor(0, 1);
  lcd.print(line2Str);

}

void wifi_loop(void)
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
  getStr += String(tempStr);
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
//  delay(16000);
}


//The setup function is called once at startup of the sketch
void setup() {

  lcdModule_setup();
  tempModule_setup();
  rtcModule_setup(false);
    wifi_setup();
}

int count = 0;
// The loop function is called in an endless loop
void loop() {

  rtcModule_loop();
  tempModule_loop();
  lcdModule_loop();
  if (count == 16) {
     wifi_loop();
     count = 0;
  }

  count++;
  // Refrech per second
  delay (DELAY_DURATION);
}



/*
 * This example is used only for ESPDuino. 
 * Based on the feature of displaying the time and temperature on LCD and updating the data to ThingSpeak
 * via WiFi, we want to replace the temperature sensor with Max6675
 */

 /*
  * Issues:
  *   1. LCD and Wifi don't work together. Wifi won't get connected. (fixed: add disconnect)
  *   2. MAX6675 and Wifi don't work together. Temperature always get 0 degree. (fixed: Don't use pin 1,3)
  */
  
#include <ESP8266WiFi.h> 
#include <DS1302.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <max6675.h>
#include "ESPDuino_U_Max6675.h"



DS1302 rtc(RTC_RST, RTC_DAT, RTC_CLK);
LiquidCrystal_I2C lcd(0x27, COL, ROW); 
MAX6675 ktc(TMP_SCK, TMP_CS, TMP_S0);


char line1Str[COL]; //RTC Time
char line2Str[COL]; //temperature
char tempStr[COL];  //temperature

WiFiClient client;

void init_step() {
  
  Serial.begin(BAUD_MONITOR);
  Serial.setDebugOutput(true);
  
}

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
  
  Wire.begin(LCD_SDA, LCD_SCL); 

  lcd.init(); // initializing the LCD
  lcd.display(); //Enable or turn on the text
  
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
  delay(DELAY_DURATION);
  lcd.setCursor(0, 1);        //set cursor position at second line
  lcd.print("LCD is ready");
  delay(DELAY_DURATION);
  
}

void tempModule_setup() {
  delay(500);
}

void wifi_setup() {

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);  
  WiFi.begin(SSID, PASSWORD); 
  Serial.println("\nConnecting to WiFi");

  //If Wifi status is not connected, it means connection failure
  while (WiFi.status() != WL_CONNECTED) {  
    Serial.print(".");   // Print '.'
    delay(DELAY_DURATION); 
  } 
  
  Serial.print("Join AP success\r\n");
  Serial.print("IP: ");
  IPAddress ip;
  ip = WiFi.localIP();
  Serial.println(ip);

  Serial.println("Connected to wifi");

}

void rtcModule_loop() {
  sprintf(line1Str, "Time: %s", rtc.getTimeStr());
}

void tempModule_loop() {
  
  float temperature = ktc.readCelsius();
  Serial.print("Deg C = "); 
  Serial.println(temperature);
  
  char temp[COL];
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

void wifi_loop() {

  Serial.println("\nStarting connection...");
  if (client.connect(TS_IP, PORT)) {
    Serial.println("connected");

    // prepare GET string
    String getStr = "GET /update?api_key=";
    getStr += WRITE_API_KEY;
    getStr +="&field1=";
    getStr += String(tempStr);
    getStr += "\r\n\r\n";
    Serial.println(getStr);

    client.println(getStr);
    client.println();
  } else {
    Serial.println("fail to connect");
  }
  
}

//The setup function is called once at startup of the sketch
void setup() {
  
  init_step();
  lcdModule_setup();
  tempModule_setup();
  rtcModule_setup(false);
  wifi_setup();

}

int count=0;
// The loop function is called in an endless loop
void loop() {
  
  rtcModule_loop();
  tempModule_loop();
  lcdModule_loop();
  if (count == WAIT_SEC) {
     wifi_loop();
     count = 0;
  }

  count++;
  // Refrech per second
  delay (DELAY_DURATION);
  
}

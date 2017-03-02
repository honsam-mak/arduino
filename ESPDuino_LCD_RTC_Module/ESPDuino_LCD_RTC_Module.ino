/*
 * This example is used only for ESPDuino. 
 * The goal is to use RTC to display the time on LCD
 */

#include <DS1302.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DELAY_DURATION 1000

DS1302 rtc(16, 15, 14);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

char* rtcTime;

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

void rtcModule_loop() {
  
  rtcTime = rtc.getTimeStr();
  
  // Refrech per second
  delay (DELAY_DURATION);
}

void lcdModule_setup() {
  Wire.begin(4, 5); // SDA: 4 (default) or 2, SCL: 5 (default) or 14

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
  delay(1000);
  lcd.setCursor(0, 1);        //set cursor position at second line
  lcd.print("LCD is ready");
}

void lcdModule_loop() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Real Time :");
  lcd.setCursor(0, 1);
  lcd.print(rtcTime);
}

//The setup function is called once at startup of the sketch
void setup() {
  lcdModule_setup();
  rtcModule_setup(false);
}

// The loop function is called in an endless loop
void loop() {
  rtcModule_loop();
  lcdModule_loop();
}

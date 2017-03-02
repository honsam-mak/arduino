#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
 
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
  lcd.print("Jonietlo Project");
  delay(1000);
  lcd.setCursor(0, 1);        //set cursor position at second line
  lcd.print("LCD is ready");

}

void loop() {

}

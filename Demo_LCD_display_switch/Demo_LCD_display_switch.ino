#include <Wire.h>

#include <LiquidCrystal_I2C.h>

// Set the pins on the I2C chip used for LCD connections:
//                    addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

const int switchPin = 2;
int switchState = 0;

void setup() {
 
 // open a serial connection to your computer
  Serial.begin(115200);
  
  //declare teh switch pin as on input
  pinMode(switchPin, INPUT);

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
  lcd.print("Hello, world");
  delay(1000);
  lcd.setCursor(0, 1);        //set cursor position at second line
  lcd.print("Honsam Mak");
  delay(8000);

  // Wait for user's input
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press button");
}

void loop() {

  switchState = digitalRead(switchPin);

  if (switchState == LOW) {
    delay(100);
    lcd.clear();
    lcd.write("OFF");
    Serial.println(switchState);
  } else {
    delay(100);
    lcd.clear();
    lcd.write("ON");
    Serial.println(switchState);
  }
}

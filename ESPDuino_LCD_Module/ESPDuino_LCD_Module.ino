/*
 * This example is used only for ESPDuino development board.
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup()
{
 Wire.begin(4, 5); // SDA: 4 (default) or 2, SCL: 5 (default) or 14

 lcd.init(); // initializing the LCD
 lcd.display(); //Enable or turn on the text
 lcd.backlight(); // Enable or Turn On the backlight 
}

void loop() {
lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Working Tutorial"); // Start Print text to Line 1
 lcd.setCursor(0, 1);
 lcd.print("HowToESP8266.com"); // Start Print text to Line 2
 delay(5000);
 lcd.clear();
 lcd.setCursor(1, 0);
 lcd.print("Tutorials good"); // Start Print text to Line 1
 lcd.setCursor(4, 1);
 lcd.print("AS FUCK"); // Start Print text to Line 2
 delay(5000);
}

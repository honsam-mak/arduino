/*
 * This example is used only for ESPDuino. 
 * The goal is to use RTC to display the time on LCD
 */
 
#include <ESP8266WiFi.h> 
#include <DS1302.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define DELAY_DURATION 1000
#define SSID        "mark"
#define PASSWORD    "6266790021"

#define TS_IP       "184.106.153.149" // api.thingspeak.com
#define WRITE_API_KEY   "6RZFFXN81EIFD1XK"

#define BAUD_MONITOR  9600
#define BAUD_ESP8266  115200

DS1302 rtc(16, 15, 14);
LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int sensorPin=A0;

// ThingSpeak Settings
//String writeAPIKey = "6RZFFXN81EIFD1XK";

char line1Str[16]; //RTC Time
char line2Str[16]; //temperature
char tempStr[16];    //temperature

WiFiClient client;

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
  delay(DELAY_DURATION);
  lcd.setCursor(0, 1);        //set cursor position at second line
  lcd.print("LCD is ready");
  delay(DELAY_DURATION);
}

void tempModule_setup() {;}

void wifi_setup() {
  Serial.begin(BAUD_MONITOR);
  Serial.setDebugOutput(true);

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

void wifi_loop() {

  Serial.println("\nStarting connection...");
  if (client.connect(TS_IP, 80)) {
    Serial.println("connected");
  }
  
  // prepare GET string
  String getStr = "GET /update?api_key=";
  getStr += WRITE_API_KEY;
  getStr +="&field1=";
  getStr += String(tempStr);
  getStr += "\r\n\r\n";
  Serial.println(getStr);

  client.println(getStr);
  client.println();
}

//The setup function is called once at startup of the sketch
void setup() {
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
  if (count == 16) {
     wifi_loop();
     count = 0;
  }

  count++;
  // Refrech per second
  delay (DELAY_DURATION);
}

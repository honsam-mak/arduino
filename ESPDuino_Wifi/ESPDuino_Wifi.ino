#include <ESP8266WiFi.h> 
#include <time.h> 

const char* ssid = "mark";  
const char* password = "6266790021";  

int timezone = 3;
int dst = 0;

void setup() {
  Serial.begin(115200); 
  Serial.setDebugOutput(true); 

  WiFi.mode(WIFI_STA);  
  WiFi.begin(ssid, password); 
  Serial.println("\nConnecting to WiFi");
 
  //If Wifi status is not connected, it means connection failure
  while (WiFi.status() != WL_CONNECTED) {  
    Serial.print(".");   // Print '.'
    delay(1000); 
  }

  // Config the time format and the url of time server
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
}

void loop() {
  time_t now = time(nullptr); //current time
  Serial.println(ctime(&now));//print the current time
  delay(1000); 
}

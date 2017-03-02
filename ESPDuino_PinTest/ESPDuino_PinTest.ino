#define PIN_LED 16
#define D0 0

void setup() {
  pinMode(PIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(D0, OUTPUT);
}

void loop() {
  digitalWrite(PIN_LED, LOW);   
  digitalWrite(D0, HIGH);
  
  delay(1000);                      
  digitalWrite(PIN_LED, HIGH);  
  digitalWrite(D0, LOW);
  delay(2000);

}

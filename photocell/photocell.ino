
int photocellPin = 2;
int photocellVal = 0;
int minLight = 180;
int ledPin = 9;
int ledState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  photocellVal = analogRead(photocellPin);
  Serial.println(photocellVal);

  if (photocellVal < minLight && ledState == 0) {
    digitalWrite(ledPin, HIGH);
    ledState = 1;
  }

  if (photocellVal > minLight && ledState == 1) {
    digitalWrite(ledPin, LOW);
    ledState = 0;
  }
  
  delay(100);
}

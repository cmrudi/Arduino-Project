int ledPin = 13;
int switchPin = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:r
  if (digitalRead(switchPin) == HIGH) {
     digitalWrite(ledPin, HIGH);
     delay(500);
     digitalWrite(ledPin,LOW);
     delay(500);
  }
  if (digitalRead(switchPin) == LOW) {
     digitalWrite(ledPin, HIGH);
     delay(50);
     digitalWrite(ledPin,LOW);
     delay(50);
  }
}


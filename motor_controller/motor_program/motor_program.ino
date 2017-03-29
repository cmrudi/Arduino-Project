
#define E1 10  // Enable Pin for Right motor 
#define E2 11  // Enable Pin for Left motor 
 
#define I1 8  // Control pin 1 for motor 1
#define I2 9  // Control pin 2 for motor 1
#define I3 12  // Control pin 1 for motor 2
#define I4 13  // Control pin 2 for motor 2
 
void setup() {
 
    pinMode(E1, OUTPUT);
    pinMode(E2, OUTPUT);
 
    pinMode(I1, OUTPUT);
    pinMode(I2, OUTPUT);
    pinMode(I3, OUTPUT);
    pinMode(I4, OUTPUT);
}
 
void loop() {
    
    analogWrite(E1, 255); // Run in half speed
    analogWrite(E2, 255); // Run in full speed

    //Maju
    moveForward();
    delay(2000);
    moveBackward();
    delay(2000);
    moveRight();
    delay(2000);
    moveLeft();
    delay(2000);
    
    stopMove();
    delay(5000); 
    // change direction
  /*
    digitalWrite(E1, LOW);
    digitalWrite(E2, LOW);
 
    delay(200);
 
    analogWrite(E1, 255);  // Run in full speed
    analogWrite(E2, 255);  // Run in half speed
 
    digitalWrite(I1, LOW);
    digitalWrite(I2, HIGH);
    digitalWrite(I3, LOW);
    digitalWrite(I4, HIGH);
 
    delay(10000);
    */
}

void moveForward(){
    digitalWrite(I1, HIGH);
    digitalWrite(I2, LOW);
    digitalWrite(I3, HIGH);
    digitalWrite(I4, LOW);
}

void moveBackward(){
    digitalWrite(I1, LOW);
    digitalWrite(I2, HIGH);
    digitalWrite(I3, LOW);
    digitalWrite(I4, HIGH);
}

void moveLeft(){
    analogWrite(E1, 255); // Run in half speed
    analogWrite(E2, 0); // Run in full speed

    digitalWrite(I1, HIGH);
    digitalWrite(I2, LOW);
    digitalWrite(I3, HIGH);
    digitalWrite(I4, LOW);
}

void moveRight(){
analogWrite(E1, 0); // Run in half speed
    analogWrite(E2, 255); // Run in full speed

  
    digitalWrite(I1, HIGH);
    digitalWrite(I2, LOW);
    digitalWrite(I3, HIGH);
    digitalWrite(I4, LOW);
}

void stopMove() {
    analogWrite(E1, 0); // Run in half speed
    analogWrite(E2, 0); // Run in full speed
  
}



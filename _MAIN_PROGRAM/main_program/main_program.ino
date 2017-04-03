#include <OneWire.h>




#define E1 10  // Enable Pin for Right motor 
#define E2 11  // Enable Pin for Left motor 
 
#define I1 8  // Control pin 1 for motor 1
#define I2 9  // Control pin 2 for motor 1
#define I3 12  // Control pin 1 for motor 2
#define I4 13  // Control pin 2 for motor 2

// DS18S20 Temperature chip i/o
OneWire ds(2);  // on pin 2

int Whole,Fract;

void setup() {
    Serial.begin(9600);
    pinMode(E1, OUTPUT);
    pinMode(E2, OUTPUT);
 
    pinMode(I1, OUTPUT);
    pinMode(I2, OUTPUT);
    pinMode(I3, OUTPUT);
    pinMode(I4, OUTPUT);
    pinMode(I4, OUTPUT);
}
 
void loop() {
    
    readTemp();
    if (Whole==28) {
      Serial.println("A");
     // writeTemp();
      //moveLeft();
    }
    else {
      //moveRight();
    }
    
}

/**
 * PROCEDURES FOR MOTOR
 */
void moveForward(){
    analogWrite(E1, 255); // Run in half speed
    analogWrite(E2, 255); // Run in full speed

    digitalWrite(I1, HIGH);
    digitalWrite(I2, LOW);
    digitalWrite(I3, HIGH);
    digitalWrite(I4, LOW);
}

void moveBackward(){
    analogWrite(E1, 255); // Run in half speed
    analogWrite(E2, 255); // Run in full speed

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

void readTemp(){
  //For conversion of raw data to C
  int HighByte, LowByte, TReading, SignBit, Tc_100;
 
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
 
  if ( !ds.search(addr)) {
      //Serial.print("No more addresses.\n");
      ds.reset_search();
      return;
  }
 
  
 
  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return;
  }
 
  
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
 
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
 
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
 
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  
  //Conversion of raw data to C
  LowByte = data[0];
  HighByte = data[1];
  TReading = (HighByte << 8) + LowByte;
  SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) // negative
  {
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25
 
  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  Fract = Tc_100 % 100;
 

  if (SignBit) // If its negative
  {
     Serial.print("-");
  }
  
  //End conversion to C
}

void writeTemp() {
  Serial.print(Whole);
  Serial.print(".");
  if (Fract < 10)
  {
     Serial.print("0");
  }
  Serial.print(Fract);
 
  Serial.print("\n");
}


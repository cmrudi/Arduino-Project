#include <OneWire.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define datax 3
#define clock 4

#define E1 10  // Enable Pin for Right motor 
#define E2 11  // Enable Pin for Left motor 
 
#define I1 8  // Control pin 1 for motor 1
#define I2 9  // Control pin 2 for motor 1
#define I3 12  // Control pin 1 for motor 2
#define I4 13  // Control pin 2 for motor 2

// DS18S20 Temperature chip i/o
OneWire ds(2);  // on pin 2
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int tempWhole,tempFract,initTemp;
int onlineInput= 5;

byte zero  = B10000001;
byte one   = B11111001;
byte two   = B00100101;
byte three = B00101001;
byte four  = B01011001;
byte five  = B00001011;
byte six   = B00000011;
byte seven = B10111001;
byte eight = B00000001;
byte nine  = B00001001;

void setup() {
    Serial.begin(9600);
    pinMode(E1, OUTPUT);
    pinMode(E2, OUTPUT);
 
    pinMode(I1, OUTPUT);
    pinMode(I2, OUTPUT);
    pinMode(I3, OUTPUT);
    pinMode(I4, OUTPUT);
    pinMode(I4, OUTPUT);

    readTemp();
    initTemp = tempWhole;

    lcd.begin(16,2);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Damkar Learning");

}
 
void loop() {
    readTemp();
    writeTemp();
    lcd.setCursor(0,5);
    String temp = tempWhole+","+tempFract+ (String)" C";
    lcd.print(temp);
    
    //BACA INPUT, masukkan ke onlineInput

    switch (onlineInput) {
      case 1: {
        moveLeft();
        //Tulis 1 di 7 segment
        shiftOut(datax, clock, LSBFIRST, one);
        break;
      }
      case 2: {
        moveForward();
        //Tulis 2 di 7 segment
        shiftOut(datax, clock, LSBFIRST, two);
        break;
      }
      case 3: {
        moveRight();
        //Tulis 3 di 7 segment
        shiftOut(datax, clock, LSBFIRST, three);
        break;
      }
      case 4: {
        moveBackward();
        //Tulis 4 di 7 segment
        shiftOut(datax, clock, LSBFIRST, four);
        break;
      }
      case 5: {
        stopMove();
        //Tulis 5 di 7 segment
        shiftOut(datax, clock, LSBFIRST, five);
        break;
      }              
    }
    //End Switch

    if((tempWhole - initTemp)>=2) {
      lcd.setCursor(0,1);
      lcd.print("PADAMKAN");
      delay(1000);
      //Tulis di LCD "PADAMKAN"
      stopMove();
      //Tulis 5 di 7 segment
      shiftOut(datax, clock, LSBFIRST, five);
      int i;
      for (i=5;i>0;i--) {
        printCount(i);
        delay(1000);
        //per detik ganti
      }
      lcd.setCursor(0,1);
      lcd.print("API PADAM");
      delay(1000);
    }
}

/**
 * PROCEDURES FOR MOTOR
 */
void moveForward(){
    analogWrite(E1, 255); // Run in full speed
    analogWrite(E2, 255); // Run in full speed

    digitalWrite(I1, HIGH);
    digitalWrite(I2, LOW);
    digitalWrite(I3, HIGH);
    digitalWrite(I4, LOW);
}

void moveBackward(){
    analogWrite(E1, 200); // Run in non full speed
    analogWrite(E2, 200); // Run in non full speed

    digitalWrite(I1, LOW);
    digitalWrite(I2, HIGH);
    digitalWrite(I3, LOW);
    digitalWrite(I4, HIGH);
}

void moveLeft(){
    analogWrite(E1, 255); // Run in full speed
    analogWrite(E2, 0); 

    digitalWrite(I1, HIGH);
    digitalWrite(I2, LOW);
    digitalWrite(I3, HIGH);
    digitalWrite(I4, LOW);
}

void moveRight(){
    analogWrite(E1, 0); 
    analogWrite(E2, 255); // Run in full speed

  
    digitalWrite(I1, HIGH);
    digitalWrite(I2, LOW);
    digitalWrite(I3, HIGH);
    digitalWrite(I4, LOW);
}

void stopMove() {
    analogWrite(E1, 0);
    analogWrite(E2, 0);
  
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
 
  tempWhole = Tc_100 / 100;  // separate off the whole and fractional portions
  tempFract = Tc_100 % 100;
 

  if (SignBit) // If its negative
  {
     Serial.print("-");
  }
  
  //End conversion to C
}

void writeTemp() {
  Serial.print(tempWhole);
  Serial.print(".");
  if (tempFract < 10)
  {
     Serial.print("0");
  }
  Serial.print(tempFract);
 
  Serial.print("\n");
  lcd.setCursor(0,1);
  lcd.write(tempWhole);
}

void printCount(int sec) {
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("PADAMKAN");
    lcd.setCursor(0,10);
    lcd.print(sec);
}


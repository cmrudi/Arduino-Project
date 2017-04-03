// Program: 16x2 LCD display and I2C module 
// blog.cirucits4you.com

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize the display at the address 0x27 
LiquidCrystal_I2C lcd ( 0x27 , 2 , 1 , 0 , 4 , 5 , 6 , 7 , 3 , POSITIVE);
 
void setup ()
{
 lcd.begin ( 16 , 2 );
}
 
void loop ()
{
  lcd.setBacklight (HIGH);
  lcd.setCursor ( 0 , 0 );
  lcd.print ( "circuits4you.com" );
  lcd.setCursor ( 0 , 1 );
  lcd.print ( "LCD and I2C module" );
  delay ( 1000 );
  lcd.setBacklight (LOW);
  delay ( 1000 );
}

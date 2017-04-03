#define data 3
#define clock 4

// use binary notation to discribe our number layouts
//byte zero  = B01111110;
//byte one   = B00000110;
//byte two   = B11011010;
//byte three = B11010110;
//byte four  = B10100110;
//byte five  = B11110100;
//byte six   = B11111100;
//byte seven = B01000110;
//byte eight = B11111110;
//byte nine  = B11110110;
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


void setup()
{
  pinMode(clock, OUTPUT); // make the clock pin an output
  pinMode(data , OUTPUT); // make the data pin an output3
}

void loop()
{
    shiftOut(data, clock, LSBFIRST, zero);
    delay(500);
    shiftOut(data, clock, LSBFIRST, one);
    delay(500);
    shiftOut(data, clock, LSBFIRST, two);
    delay(500);
    shiftOut(data, clock, LSBFIRST, three);
    delay(500);
    shiftOut(data, clock, LSBFIRST, four);
    delay(500);
    shiftOut(data, clock, LSBFIRST, five);
    delay(500);
    shiftOut(data, clock, LSBFIRST, six);
    delay(500);
    shiftOut(data, clock, LSBFIRST, seven);
    delay(500);
    shiftOut(data, clock, LSBFIRST, eight);
    delay(500);
    shiftOut(data, clock, LSBFIRST, nine);
    delay(500);
}

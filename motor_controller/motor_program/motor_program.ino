/* 
 * Program utk Famosa Studio Robotik Kit ini diadaptasi dari
 * program yang dibuat oleh
 * George Frick (george.frick@gmail.com)
 * dan dipublikasikan di sini:
 * http://georgefrick.blogspot.com/2010/02/arduino-robot-2-wandering-tank.html
 * 
 * Famosa Studio Robotik Kit adalah produk robotik dari Toko Online Famosa Studio.
 * Dilengkapi Arduino Uno, Motor Shield 1A, Standard Servo, Micro Servo dan Sharp GP2D12. 
 *
 */  
#include <Servo.h>  
  
// Two motors, a servo, and an IR range sensor.  
Servo sensorServo, centerServo;  
const int irPin = A0; // analog pin 0
  
// Timing. I don't like using delay.  
unsigned long tCnt = 0;  
unsigned long tStart = 0;  
unsigned long tDelta = 0;  
unsigned long tTurn = 0;  
unsigned long tTurnDur = 400;  //turn duration
  
int state;                  // Current Robot State  
int lastState;              // Previous Robot State  
int servoPos;               // Position to send servo  
int centerPos;              // Center position to send servo  
int servoDirection;         // Direction servo is turning  
int lastDetectionAngle;     // Position of servo at last IR detect.  
int sensorValue = 0;        // sensor value
const int minDistance = 20; // minimum distance 20 cm
  
// Constants for state of robot wheel  
const int STATE_FORWARD = 1;  
const int STATE_TURN_RIGHT = 2;  
const int STATE_BACKWARD = 3;  
const int STATE_TURN_LEFT = 4;  
  
// Constants for Servo.  
const int DIR_LEFT = 0;  
const int DIR_RIGHT = 1;  
const int MIN_DEGREE = 40;  
const int MAX_DEGREE = 140;  

// Constants for robot wheel.  
const int RIGHTSPEED = 255;  //right motor speed
const int LEFTSPEED  = 255;  //left motor speed 
int motorR = 5;       //right motor
int dirmotorR = 4;    //motor direction
int motorL = 6;       //left motor
int dirmotorL = 7;    //motor direction

// Using Ultrasonic Range Sensor HC-SR04
#include <NewPing.h>

#define TRIGGER_PIN  2  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     3  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 300 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

  
/* 
 * Initializes everything. Is run once. 
 */  
void setup() {  
  Serial.begin(9600);           // set up Serial library at 9600 bps  
  sensorServo.attach(9);  // attaches the servo on pin 9  
  centerServo.attach(10);  // attaches the servo on pin 10  
  tStart = millis();  
  lastState = state = STATE_FORWARD;  
  servoPos = 90;  
  centerPos = 90;  
  servoDirection = DIR_RIGHT;  
  sensorServo.write(servoPos);  
  //state = 0; // Uncomment to have robot not move wheel.  
}  
  
/* 
 * Runs continuously. 
 * 1. Update Servo. 
 * 2. Check IR range sensor. 
 * 3. Move Robot 
 */  
 
void loop() {  
  tDelta = millis() - tStart;  
  tCnt += tDelta;  
  tStart += tDelta;  
  
  // Tell the servo to move 2 degrees every 25 ticks.  
  if( tCnt > 25 ) {  
    tCnt = 0;  
    if( servoDirection == DIR_LEFT ) {  
      servoPos -= 2;  
    } else if( servoDirection == DIR_RIGHT) {  
      servoPos += 2;  
    }  
  
    // Servo position will be beyond desired angles, turn around.  
    if( servoPos >= MAX_DEGREE ) {  
      servoDirection = DIR_LEFT;  
    } else if( servoPos <= MIN_DEGREE ) {  
      servoDirection = DIR_RIGHT;  
    }  
    sensorServo.write(servoPos);  
    centerServo.write(centerPos);  
  }  
  
  // Allows disabling of tracks by setting state to 0.  
  if(state == 0) {  
    moveRobot();  
    return;  
  }  
  
  //sensorValue = read_gp2d12_range(irPin);
  int sensorValue = sonar.ping_cm(); // Send out the ping, get the results in centimeters.
  delay(25);
  if (sensorValue <= minDistance && sensorValue > 0) {
    lastDetectionAngle = servoPos;  
    state = STATE_BACKWARD;  
  } else {  
    if( state == STATE_BACKWARD ) {  
      if( lastDetectionAngle > 105 ) { // right  
        state = STATE_TURN_LEFT;  
        tTurn = tTurnDur;
      } else if( lastDetectionAngle < 75 ) { // left  
        state = STATE_TURN_RIGHT;  
        tTurn = tTurnDur;
      } else { // center  
        state = STATE_TURN_RIGHT; // for now, turn right by default.  
        tTurn = tTurnDur;
      }  
    } else if ( state == STATE_TURN_RIGHT || state == STATE_TURN_LEFT ) {  
      if (tTurn > tDelta) {
        tTurn -= tDelta;
      }
      if( tTurn <= 10 || tTurn < tDelta ) {  
        state = STATE_FORWARD;  
      }  
    } else {  
      state = STATE_FORWARD;  
    }  
  }  
  
  moveRobot();  
}  
  
/* 
 * Uses the state of the robot to move wheels accordingly 
 */  
void moveRobot() {  
  
  // The motors seemed to respond better if they receive a stop before a switch in direction.  
  if( state != lastState ) {  
    RMotor(0,false); //stopped
    LMotor(0,false); //stopped
  }  
  
  switch( state ) {  
    default:  
      return; // helps test, state 0 = dont move.  
    case STATE_FORWARD: {  
      RMotor(RIGHTSPEED,true); // turn it on going forward
      LMotor(LEFTSPEED,true);  // turn it on going forward
      break;  
    }  
    case STATE_BACKWARD: {  
      RMotor(RIGHTSPEED,false); // turn it on going backward
      LMotor(LEFTSPEED,false);  // turn it on going backward
      break;  
    }  
    case STATE_TURN_RIGHT: {  
      RMotor(RIGHTSPEED,false); // turn it on going right
      LMotor(LEFTSPEED,true);   // turn it on going right
      break;  
    }  
    case STATE_TURN_LEFT: {  
      RMotor(RIGHTSPEED,true); // turn it on going left
      LMotor(LEFTSPEED,false); // turn it on going left
      break;  
    }  
  }  
  
  lastState = state;  
}  

// right motor
void RMotor(int pwmspeed, boolean forward) {
  analogWrite(motorR, pwmspeed);
  if (forward) {
    digitalWrite(dirmotorR, HIGH);
  }
  else {
    digitalWrite(dirmotorR, LOW);
  }
}

//left motor
void LMotor(int pwmspeed, boolean forward) {
  analogWrite(motorL, pwmspeed);
  if (forward) {
    digitalWrite(dirmotorL, HIGH);
  }
  else {
    digitalWrite(dirmotorL, LOW);
  }
}

// Sharp GP2D12 read distance function  
float read_gp2d12_range(byte pin)
{
  int tmp;

  tmp = analogRead(pin);
  if (tmp < 3)
    return -1; // invalid value
  return (6787.0 /((float)tmp - 3.0)) - 4.0; // output in cm
} 
  
/* EOF */

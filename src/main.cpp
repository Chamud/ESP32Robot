#include <Arduino.h>

#define enL 16
#define in1L 17
#define in2L 5
#define in3R 18
#define in4R 19
#define enR 21

#define X 35
#define Y 34

int motorIdle = 75;
int joystickIdle = 20;
int motorSpeedPrc = 60;
int turnSpeedPrc = 80;

int motorSpeedL = 0;
int motorSpeedR = 0;
int jX = 0;
int jY = 0;

void setup() {
  Serial.begin(115200);
  pinMode(enL, OUTPUT);
  pinMode(enR, OUTPUT);
  pinMode(in1L, OUTPUT);
  pinMode(in2L, OUTPUT);
  pinMode(in3R, OUTPUT);
  pinMode(in4R, OUTPUT);
  pinMode(X, INPUT);
  pinMode(Y, INPUT);
}

void runMotor(int motorSpeed, int in1Pin, int in2Pin, int enPin) {

  motorSpeed = constrain(motorSpeed, -255, 255);

  // Digital control
  if (motorSpeed < 0) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
  } else if (motorSpeed > 0) {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
  } else{
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
  }

  // Analog control
  motorIdle = constrain(motorIdle, 0, 100);
  motorSpeed = map(abs(motorSpeed), 0, 255, motorIdle, 255);
  if (motorSpeed > motorIdle){
    analogWrite(enPin, motorSpeed);
  } else{
    analogWrite(enPin, 0);
  }  
}

int readJoyStick(int pin) {
  int val = pulseIn(pin, HIGH);
  int mappedValue = map(val, 2012, 989, 255, -255);
  joystickIdle = constrain(joystickIdle, 0 , 100);
  if(abs(mappedValue) < joystickIdle){
    return 0;
  }
  return mappedValue;
}

void setMotorSpeed() {
  motorSpeedL = jX;
  motorSpeedR = jX;
  turnSpeedPrc = constrain(turnSpeedPrc, 20, 100);
  motorSpeedPrc = constrain(motorSpeedPrc, 20, 100);

  jY = jY * turnSpeedPrc / 100;
  if (jY > 0) {
    if(jX>0){
      motorSpeedR -= jY;
    }else if(jX<0){
      motorSpeedR += jY;
    }else{
      motorSpeedL = jY;
      motorSpeedR = -jY;
    }
  }else if(jY<0){
    if(jX>0){
      motorSpeedL += jY;
    }else if(jX<0){
      motorSpeedL -= jY;
    }else{
      motorSpeedL = jY;
      motorSpeedR = -jY;
    }
  }
  motorSpeedL  = motorSpeedL * motorSpeedPrc / 100;
  motorSpeedR  = motorSpeedR * motorSpeedPrc / 100;
}

void loop() {
  jX = readJoyStick(X);
  jY = readJoyStick(Y);

  setMotorSpeed();

  runMotor(motorSpeedL, in1L, in2L, enL);
  runMotor(motorSpeedR, in3R, in4R, enR);

  Serial.println((String)jX+' '+(String)jY+' '+(String)motorSpeedL+' '+(String)motorSpeedR);

  delay(50);
}
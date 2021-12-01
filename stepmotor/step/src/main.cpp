#include <Arduino.h>

// step modes
uint8_t halfStepMode[8][4] = {
  {1, 0, 0 ,0},
  {1, 1, 0 ,0},
  {0, 1, 0 ,0},
  {0, 1, 1 ,0},
  {0, 0, 1 ,0},
  {0, 0, 1 ,1},
  {0, 0, 0 ,1},
  {1, 0, 0, 1}
};

uint8_t fullStepMode[4][4] = {
  {1, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1},
};
// end step modes

// types
  //pins
typedef struct{
  uint8_t pin1;
  uint8_t pin2;
  uint8_t pin3;
  uint8_t pin4;
}StepMotorPins_t;

  // direction
typedef enum{
  clockwise, otherwise
}direction_t;

  //step Mode
typedef enum{
  fullStep, halfStep
}stepMode_t;

  //stepper
typedef struct{
  StepMotorPins_t pins;
  uint8_t acc; // 1 - 255
  direction_t direction;
  stepMode_t stepMode;
}stepper;
// end types

// func
void initStepper(
    stepper* s,
    uint8_t motorPin1, uint8_t motorPin2, uint8_t motorPin3, uint8_t motorPin4,
    stepMode_t modeStep,
    direction_t direction,
    uint8_t speed
  )
{
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  s->pins.pin1 = motorPin1;
  s->pins.pin2 = motorPin2;
  s->pins.pin3 = motorPin3;
  s->pins.pin4 = motorPin4;
  s->acc = (speed > 254) ? (2) : (256 - speed);
  s->direction = direction;
  s->stepMode = modeStep;
}

// clockwise Half and Full mode
void stepWriteDCH(stepper* s, uint8_t j){
  digitalWrite(s->pins.pin4, halfStepMode[j][0]);
  digitalWrite(s->pins.pin3, halfStepMode[j][1]);
  digitalWrite(s->pins.pin2, halfStepMode[j][2]);
  digitalWrite(s->pins.pin1, halfStepMode[j][3]);
  delay(s->acc);
}

void stepWriteDCF(stepper* s, uint8_t j){
  digitalWrite(s->pins.pin4, fullStepMode[j][0]);
  digitalWrite(s->pins.pin3, fullStepMode[j][1]);
  digitalWrite(s->pins.pin2, fullStepMode[j][2]);
  digitalWrite(s->pins.pin1, fullStepMode[j][3]);
  delay(s->acc);
}

void stepDirectionClockwise(stepper* s){
  if (s->stepMode == halfStep){
    for(uint8_t j = 0; j < 8; stepWriteDCH(s, j++));
  }else if(s->stepMode == fullStep){
    for(uint8_t j = 0; j < 4; stepWriteDCF(s, j++));
  } 
}
// end clockwise Half and Full mode

// otherwise Full and Half mode
void stepWriteCCH(stepper* s, uint8_t j){
  digitalWrite(s->pins.pin1, halfStepMode[j][0]);
  digitalWrite(s->pins.pin2, halfStepMode[j][1]);
  digitalWrite(s->pins.pin3, halfStepMode[j][2]);
  digitalWrite(s->pins.pin4, halfStepMode[j][3]);
  delay(s->acc);
}

void stepWriteCCF(stepper* s, uint8_t j){
  digitalWrite(s->pins.pin1, fullStepMode[j][0]);
  digitalWrite(s->pins.pin2, fullStepMode[j][1]);
  digitalWrite(s->pins.pin3, fullStepMode[j][2]);
  digitalWrite(s->pins.pin4, fullStepMode[j][3]);
  delay(s->acc);
}

void stepCounterClockwise(stepper* s){
  if(s->stepMode == halfStep){
    for (size_t j = 0; j < 8; stepWriteCCH(s, j++));
  }else if(s->stepMode == fullStep){
    for (size_t j = 0; j < 4; stepWriteCCF(s, j++));  
  }
}
// end otherwise Half and Full mode

void goStep(stepper* s){
  if(s->direction == clockwise) stepDirectionClockwise(s);
  else if(s->direction == otherwise) stepCounterClockwise(s);
}
// end func

stepper s;
void setup() {
  initStepper(&s, 9, 10, 11, 12, fullStep, clockwise, 254);
}

void loop() {
  goStep(&s);
}
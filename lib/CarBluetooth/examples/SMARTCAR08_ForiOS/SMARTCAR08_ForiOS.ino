//test 1:2015.08.15
#include <car_bluetooth.h>
#include <SoftwareSerial.h>   //Software Serial Port
#include <String.h>
#include "QuadMotorDriver.h"

#define RxD 2//D2 of Arduino should connect to TX of the Serial Bluetooth module
#define TxD 4//D4 of Arduino should connect to RX of the Serial Bluetooth module
CarBluetooth bluetooth(RxD, TxD);
#define CMD_INVALID     0XFF
#define CMD_FORWARD     'a'
#define CMD_TURN_RIGHT 'b'

#define CMD_BACKWARD    'c'

#define CMD_TURN_LEFT  'd'
#define CMD_SPEED_UP    'e'
#define CMD_SPEED_DOWN  'g'

#define CMD_SLIDE_RIGHT 'f'
#define CMD_SLIDE_LEFT  'h'


#define SPEED_STEPS 20
uint8_t speed0 = 100;



void setup(){
  motordriver.init(3,5,6,11);
  motordriver.setSpeed(255);
  bluetooth.waitConnected();
}
uint8_t bt_command;
uint8_t car_mode;
#define NORMAL 0

#define CAR_STOP 0
#define CAR_FORWARD 1
#define CAR_BACK 2
uint8_t car_status = CAR_STOP;
uint8_t new_status = car_status;

void loop(){
  bt_command = bluetooth.readByte();
  if(bt_command != CMD_INVALID){
  controlCar(bt_command);
  }
}


void controlCar(uint8_t cmd){
   switch(cmd)
  {
    case CMD_FORWARD:   motordriver.goForward();break;
    case CMD_TURN_RIGHT: motordriver.turnRight();break;
    case CMD_BACKWARD:    motordriver.goBackward(); break;
    case CMD_TURN_LEFT:  motordriver.turnLeft(); break;
    case CMD_SPEED_UP:    speedUp();break;
    case CMD_SPEED_DOWN:  speedDown();break;
    case CMD_SLIDE_RIGHT: motordriver.slideRight();break;
    case CMD_SLIDE_LEFT:  motordriver.slideLeft();break;
    default: motordriver.stop();break;
  }
  
}

void speedUp(){
  if(speed0 < 236)speed0 += SPEED_STEPS;
  else speed0 = 255;
  motordriver.setSpeed(speed0);
}

void speedDown(){
  if(speed0 > 70)speed0 -= SPEED_STEPS;
  else speed0 = 50;
  motordriver.setSpeed(speed0);
}

//test 1:2015.08.15
#include <car_bluetooth.h>
#include <SoftwareSerial.h>   //Software Serial Port
#include <String.h>
#include "QuadMotorDriver.h"


#define RxD 2//D2 of Arduino should connect to TX of the Serial Bluetooth module
#define TxD 4//D4 of Arduino should connect to RX of the Serial Bluetooth module
CarBluetooth bluetooth(RxD, TxD);
#define CMD_INVALID     0XFF
#define CMD_FORWARD     'F'
#define CMD_TURN_RIGHT 'I'
#define CMD_SLIDE_RIGHT 'R'
#define CMD_BACKWARD    'B'

#define CMD_TURN_LEFT  'G'
#define CMD_SLIDE_LEFT 'L'
#define CMD_STOP        'S'

#define SPEED_STEPS 50
uint8_t speed0 = 255;

void setup(){
  motordriver.init(3,5,6,11);	
  
  bluetooth.waitConnected();
  motordriver.setSpeed(255);
}
uint8_t bt_command;

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
    case CMD_FORWARD:     motordriver.goForward();break;
	case CMD_SLIDE_RIGHT:     motordriver.slideRight();break;
    case CMD_TURN_RIGHT: 
	              motordriver.turnRight();
                  // delay(200); 
                  break;
    case CMD_BACKWARD:    motordriver.goBackward(); break;
	case CMD_SLIDE_LEFT:    motordriver.slideLeft(); break;
    case CMD_TURN_LEFT:  motordriver.turnLeft();
                          // delay(200);
                          break;
  case CMD_STOP:        motordriver.stop();break;
  default: break;
  }
  if((cmd>='0')&&(cmd<='9'))
  {
  	speed0 = cmd-0x30;
	Serial.print(speed0);
	Serial.print(">");
	speed0 = map(speed0, 0, 9, 0, 255);
	Serial.println(speed0);
	motordriver.setSpeed(speed0);
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

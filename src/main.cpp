//test 1:2015.08.15
#include <car_bluetooth.h>
#include <SoftwareSerial.h>   //Software Serial Port
#include "QuadMotorDriver.h"
#include <Ultrasonic.h>

#define TRIGGER_PIN  7//connect Trip of the Ultrasonic Sensor moudle to D5 of Arduino 
                      //and can be changed to other ports
#define ECHO_PIN     8

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

#define SPEED_STEPS 20
uint32_t max_speed = 100;
uint32_t min_speed = 50;
uint8_t speed0 = 20;
uint8_t distanceThreshold = 40; // in cm
float distance;
String state = "Forward";

void speedUp(){
  if(speed0 < max_speed){
    speed0 += SPEED_STEPS;
  }
  else speed0 = max_speed;
  motordriver.setSpeed(speed0);
}

void speedDown(){
  if(speed0 > min_speed){
    speed0 -= SPEED_STEPS;
  }
  else speed0 = min_speed;
  motordriver.setSpeed(speed0);
}

void setup(){
  motordriver.init(3,5,6,11);	
  motordriver.setSpeed(255);
  Serial.begin(9600);
  Serial.println("Setup complete.");
}

void loop(){
  distance = ultrasonic.convert(ultrasonic.timing(), Ultrasonic::CM);
  
  if (state == "Forward") {
    motordriver.goForward();
    if (distance < distanceThreshold) {
      state = "Turn";
      Serial.println("State changed to Turn");
    } 
    else {
      speedUp();
    }
  }

  else if (state == "Turn")
  {
    motordriver.turnLeft();
    if (distance >= distanceThreshold) {
      state = "Forward";
      Serial.println("State changed to Forward");
    }
  }
}


/*
 // HCSR04Ultrasonic/examples/UltrasonicDemo/UltrasonicDemo.ino
 // for 4WD Bluetooth by catalex
 // Store: http://www.aliexpress.com/store/1199788
//      http://dx.com
 */





// void setup()
//   {
//   Serial.begin(9600);
//   }

// void loop()
//   {
//   float cmMsec, inMsec;
//   long microsec = ultrasonic.timing();

//   cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
//   inMsec = ultrasonic.convert(microsec, Ultrasonic::IN);
//   Serial.print("MS: ");
//   Serial.print(microsec);
//   Serial.print(", CM: ");
//   Serial.print(cmMsec);
//   Serial.print(", IN: ");
//   Serial.println(inMsec);
//   delay(1000);
//   }


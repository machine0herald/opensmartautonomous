//test 1:2015.08.15
#include <car_bluetooth.h>
#include <SoftwareSerial.h>   //Software Serial Port
#include "QuadMotorDriver.h"
#include <Ultrasonic.h>

#define TRIGGER_PIN_L  7//connect Trip of the Ultrasonic Sensor moudle to D5 of Arduino 
                      //and can be changed to other ports
#define ECHO_PIN_L     8

#define TRIGGER_PIN_R  2//connect Trip of the Ultrasonic Sensor moudle to D5 of Arduino 
                      //and can be changed to other ports
#define ECHO_PIN_R     4

Ultrasonic ultrasonic_left(TRIGGER_PIN_L, ECHO_PIN_L);
Ultrasonic ultrasonic_right(TRIGGER_PIN_R, ECHO_PIN_R);

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
  Serial.begin(9600);
  Serial.println("Setup complete.");
}

// Define a struct to hold the cruise controller output
struct CruiseControlOutput {
  int turn_speed;
  int forward_speed;
};

CruiseControlOutput twist;

// Modify the cruise_controller function to return the struct
CruiseControlOutput cruise_controller(float left_distance, float right_distance) {
  float k_p_turn = 1.9;
  float k_p_forward = 0.9;
  float k_p_turn_med = 2.8;

  int turn_speed = static_cast<int>(
    (k_p_turn * (left_distance - right_distance)) +
            (k_p_turn_med / (left_distance + right_distance))
          ); // Explicit initialization
  int forward_speed = static_cast<int>(k_p_forward * (left_distance + right_distance)); // Explicit initialization

  return {turn_speed, forward_speed};
}

void twist_motor_speed(int turn_speed, int forward_speed){
  int speed_A = turn_speed + forward_speed; 
  int speed_B = turn_speed + forward_speed;
  int speed_C = turn_speed - forward_speed;
  int speed_D = turn_speed - forward_speed;
  int speeds[] = {speed_A, speed_B, speed_C, speed_D};

  for (int motorID = 0; motorID < 4; motorID++){ 
    motordriver.setSpeed(abs(speeds[motorID]), motorID);
    Serial.print("Motor ");Serial.print(motorID);
    Serial.print(" speed: ");Serial.println(speeds[motorID]);

    if (speeds[motorID] >= 0){
      motordriver.rotate(MOTOR_CLOCKWISE, motorID);
    }
    else {
      motordriver.rotate(MOTOR_ANTICLOCKWISE, motorID);
    }
  }
}

void  loop(){
  float distance_left = ultrasonic_left.convert(ultrasonic_left.timing(), Ultrasonic::CM);
  float distance_right = ultrasonic_right.convert(ultrasonic_right.timing(), Ultrasonic::CM);
  // Serial.print("Left Distance: "); Serial.print(distance_left); Serial.print(" cm\t");
  // Serial.print("Right Distance: "); Serial.print(distance_right); Serial.println(" cm");
  twist = cruise_controller(distance_left, distance_right);
  twist_motor_speed(twist.turn_speed, twist.forward_speed);
  delay(50);
}
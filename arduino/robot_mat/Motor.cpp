#include "Motor.h"

Motor::Motor(int pin_power,int pin_direction,int pin_position) : pid(0.1,0.9,0.2) 
{
  this->pin_power=pin_power;
  this->pin_direction=pin_direction;
  this->pin_positoin=pin_position;
  this->position=0;   
  this->velocity=0;
  this->previous_position=0;
}

void Motor::setup() {
  pinMode(pin_power, OUTPUT);
  pinMode(pin_direction, OUTPUT);
  pinMode(pin_positoin, INPUT_PULLUP);      
}

int Motor::move(int velocity, int direction) {
  this->velocityTarget=velocity;
  //Setup the PID target.
  pid.setTarget(velocityTarget); 

  //Setup direction
  if( direction == 0 ) {
    digitalWrite(pin_direction,LOW);
  } else if( direction == 1 ) {
    digitalWrite(pin_direction,HIGH);
  } 
  //Move
  move_velocity(velocityTarget);
  return 0;             
}   

int Motor::move_velocity(int velocity)
{
  this->velocityDemanded=velocity;
  //Convert from velocity to duty
  int duty = ceil(velocityDemanded*power_by_velocity_factor);  
  if (duty<0) duty=0;
  if (duty>1023) duty=1023;

  //Serial.print("duty:");
  //Serial.println(duty);
  
  //Send to the hardware both duty and direcction
  analogWrite(pin_power,duty); 
 
}

void Motor::stop() {
  analogWrite(pin_power,0);
}

void Motor::updatePosition() {
  this->position=this->position+1;   
}

void Motor::updateVelocity() {
  this->velocityCurrent =  (position - previous_position) / ( 1.0 );
  this->previous_position=position;     

/*
  if (pin_power==5) {
    Serial.print(velocityTarget);
    Serial.print("\t");
    Serial.print(velocityCurrent);
    Serial.print("\t");
    Serial.print(velocityDemanded);
    Serial.print("\n");
  }
*/

  //Update the current velocity.
  pid.setInput(this->velocityCurrent);
  //Update the demanded velocity.
  this->velocityDemanded = pid.compute();
  //Move demanded.
  move_velocity(velocityDemanded);
}

int Motor::getPosition() {
  return this->position;
}

float Motor::getVelocityTarget() {
  return this->velocityTarget;
}

float Motor::getVelocityDemanded() {
  return this->velocityDemanded;
}

float Motor::getVelocityCurrent() {
  return this->velocityCurrent;
}

void Motor::configurePid(double Kp, double Ki, double Kd) {
  pid.setTunings(Kp,Ki,Kd);
}


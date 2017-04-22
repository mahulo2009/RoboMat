#include "motor.h"

Motor::Motor(int pin_power,int pin_direction,int pin_positoin) {
  this->pin_power=pin_power;
  this->pin_direction=pin_direction;
  this->pin_positoin=pin_positoin;
  this->position=0;  
  this->previous_time=0;
  this->velocity=0;

}

void Motor::setup() {
  pinMode(pin_power, OUTPUT);
  pinMode(pin_direction, OUTPUT);
  pinMode(pin_positoin, INPUT_PULLUP);      
}

int Motor::move(double velocity, int direction,int duration) {
  this->duration=duration;
  int duty = velocity*power_by_velocity_factor;  
  if ( (duty <0) || (duty >1023) ) {
    return -1;
  }
  if ( (direction <0) || (direction >1) ) {
    return -1;
  }
  analogWrite(pin_power,duty);
  if( direction == 0 ) {
    digitalWrite(pin_direction,LOW);
  } else if( direction == 1 ) {
    digitalWrite(pin_direction,HIGH);
  } 
  return 0;             
}   

void Motor::stop() {
  duration=duration-1;
  if (duration <=0) {
    analogWrite(pin_power,0);
  }
}

void Motor::updatePosition() {

  long time_now = millis();
  if (time_now > this->previous_time + 15 ) {
    this->position=this->position+1;        
    this->previous_time=time_now;
  }
    
}

int Motor::getPosition() {
  return this->position;
}





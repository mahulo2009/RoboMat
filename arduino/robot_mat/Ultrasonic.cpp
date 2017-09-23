#include "Ultrasonic.h"

Ultrasonic::Ultrasonic(int pin_trigger,int pin_echo): 
  pin_trigger_(pin_trigger),
  pin_echo_(pin_echo) {
    
  pinMode(pin_trigger, OUTPUT);
  pinMode(pin_echo, INPUT);
}


double Ultrasonic::updateDistance()  {
  digitalWrite(pin_trigger_, LOW);
  delayMicroseconds(4);
  digitalWrite(pin_trigger_, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_trigger_, LOW);
  
  long duration = pulseIn(pin_echo_, HIGH);
  
  distance_ = duration * 10 / 292/ 2;
  
  Serial.print("distance:");
  Serial.print("\t");
  Serial.print(distance_ );
  Serial.print("\n");
  
  return distance_; 
}


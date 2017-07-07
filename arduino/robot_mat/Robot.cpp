#include "Robot.h"

Robot::Robot() {
  engine_[0] = new Motor(5,0,14);
  engine_[1] = new Motor(4,2,12);
}

void Robot::setup() {
  for (int i=0;i<2;i++) {
      engine_[i]->setup();
  }  
}

int Robot::move(int number, int power, int direction) {
  if ( (number <0) || (number >1) ) {
    return -1;
  }  
  return engine_[number]->move(power,direction);
}

int Robot::stop(int number) {
  if ( (number <0) || (number >1) ) {
    return -1;
  }  
  engine_[number]->stop();
  return 0;
}

int Robot::updatePosition(int number) {
  if ( (number <0) || (number >1) ) {
    return -1;
  }  
  engine_[number]->updatePosition();
  return 0;  
}

int Robot::updateVelocity() {
  for (int i=0;i<2;i++) {
    engine_[i]->updateVelocity();    
  }
  return 0;  
}


int Robot::getPosition(int number) {
  return engine_[number]->getPosition();
}

float Robot::getVelocity(int number) {
  return engine_[number]->getVelocity();
}

int Robot::move(int power, int direction) {
  for (int i=0;i<2;i++) {
    engine_[i]->move(power,direction);
  }  
  return 0;
}

int Robot::stop() {
  for (int i=0;i<2;i++) {
    engine_[i]->stop();
  }  
  return 0;
}







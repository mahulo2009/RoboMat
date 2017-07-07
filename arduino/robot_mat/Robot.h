#ifndef Robot_H
#define Robot_H

#include "Motor.h"

class Robot {
  public:
    Robot();
    void setup();
    int move(int number, int power, int direction);
    int stop(int number);
    int updatePosition(int number);
    int updateVelocity();
    int getPosition(int number);
    float getVelocity(int number);
    int move(int power, int direction);
    int stop();    
  private:
    Motor *engine_[2];
};

#endif



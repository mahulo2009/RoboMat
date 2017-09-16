#ifndef Motor_H
#define Motor_H

#include <ESP8266WiFi.h>
#include "Encoder.h"

#define DEBUG 1

/**
 *
 */
class Motor {

  public:
     /**
     *
     */
    Motor(int pin_power,int pin_direction,int pin_position);
    /**
     *
     */
    void move(double velocity);
    /**
     *
     */
    void stop();
     /**
     * 
     */
    void updateEncoder();
     /**
     *
     */
    void updateControlLoopLowLevel();
    /**
     *
     */
    void updateControlLoopHighLevel();
    /**
     *
     */
    double getVelocity(double dt);
     
  private:
  
    Encoder encoder_;
    /**
     * 
     */
    double angle_;    
    /**
     * 
     */
    double previous_angle_;
    /**
     * 
     */
    const double angle_per_tic_ = (TWO_PI) / 21.0;  
};
#endif


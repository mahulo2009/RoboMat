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
    double getPosition();
    /**
     *
     */
    double getDistance();
   
  private:
  
    Encoder encoder_;
    /**
     * 
     */
    double position_;    
    /**
     * 
     */
    double previous_position_;
    /**
     * 
     */
    const double distance_per_count_ = (TWO_PI * 0.035) / 20;  
};
#endif


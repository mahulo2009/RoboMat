#ifndef motor_H
#define motor_H

#include <ESP8266WiFi.h>

class Motor {
  private:
    int pin_power;
    int pin_direction;
    int pin_positoin;
    int duration;
    int position;    
    int velocity;
    long previous_time;   

    double power_by_velocity_factor =  1023.0/1.25;
    
  public:
    /**
     * 
     */
    Motor(int pin_power,int pin_direction,int pin_positoin);
    /**
     * 
     */
    void setup();
    /**
    * 
    */
    int move(double velocity, int direction,int duration);
    /**
     * 
     */
    void stop();
    /**
     * 
     */
    void updatePosition();

    /**
     * 
     */
    int getPosition();
};

#endif

#ifndef motor_H
#define motor_H

#include <ESP8266WiFi.h>

class Motor {
  private:
    int pin_power;
    int pin_direction;
    int pin_positoin;
    int position;    
    int velocity;
    long previous_time;    
    
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
    int move(int power, int direction);
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

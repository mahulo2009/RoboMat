#ifndef Motor_H
#define Motor_H

#include <ESP8266WiFi.h>

class Motor {
  private:
    int pin_power;
    int pin_direction;
    int pin_positoin;
    int position;    
    float velocity;
    long previous_position;    
    
    
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
    void updateVelocity();

    /**
     * 
     */
    int getPosition();

    /**
     * 
     */
    float getVelocity();

    private:
    
};

#endif

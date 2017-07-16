#ifndef Motor_H
#define Motor_H


#include <ESP8266WiFi.h>
#include "pid.h"

class Motor {
  private:
    int pin_power;
    int pin_direction;
    int pin_positoin;
    int position;    
    float velocity;
    long previous_position;    
    PID pid;

    float velocityTarget;
    float velocityDemanded;
    float velocityCurrent;
    
    
    double power_by_velocity_factor =  1023.0/10.0; //TODO The factor is related to update frecuency.
    
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
    int move(int velocity, int direction);

    int move_velocity(int velocity);
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
    float getVelocityTarget();
    float getVelocityDemanded();
    float getVelocityCurrent();

    void configurePid(double Kp, double Ki, double Kd);

    private:
    
};

#endif


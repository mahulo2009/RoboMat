#ifndef Encoder_H
#define Encoder_H

#include <ESP8266WiFi.h>
#include "pid.h"

#define DEBUG 1

/**
 * 
 */
class Encoder {

  public:
    /**
     * 
     */
    Encoder(int pin_power,int pin_direction,int pin_position);
    /**
     * 
     */
    void move(long velocity);
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
    void updateControlLoop();
     /**
     * 
     */
    long getEncoder();
    /**
     * 
     */
    long getEncoderVelocityTarget();
     /**
     * 
     */
    long getEncoderVelocityDemanded();
     /**
     * 
     */
    long getEncoderVelocityCurrent();
    
  private:
    /**
     * 
     */
    PID pid_;
    /**
     * 
     */
    int pin_power_;
    /**
     * 
     */
    int pin_direction_;
    /**
     * 
     */
    int pin_position_;
    /**
     * 
     */
    long encoder_;    
     /**
     * 
     */
    long previous_encoder_;    
    /**
     * 
     *  1 	Forward
     * -1 	Backward
     */
    int direction_;
     /**
     * 
     */
    long velocity_encoder_;
     /**
     * 
     */
    long velocity_encoder_target_;
    /**
     * 
     */
    long velocity_encoder_demanded_;
    /**
     * 
     */
    long velocity_encoder_current_;
    /**
     * 
     */
    const long max_velocity_ = 21; //Max value per update period TODO CHECK THIS VALUE
    /**
     * 
     */
    const double max_pwm_ = 1023.0;
    /**
     * 
     */
    const double power_by_velocity_factor_ =  max_pwm_/max_velocity_;
    /**
     * 
     */
    void setup_();
    /**
     * 
     */
    void move_velocity_(long velocity);
    
};

#endif

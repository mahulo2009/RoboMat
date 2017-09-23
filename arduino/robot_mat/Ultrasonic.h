#ifndef Ultrasonic_H
#define Ultrasonic_H

#include <ESP8266WiFi.h>

/**
 * 
 */
class Ultrasonic {

  public:
    /**
     * 
     */
    Ultrasonic(int pin_trigger,int pin_echo);
    /*
     * 
     */
    double updateDistance();
    /**
     * 
     */
    inline double distance() {
      return distance_;
    }

  private:
    /**
     * 
     */
    int pin_trigger_;
    /**
     * 
     */
    int pin_echo_;
    /**
     * 
     */
    double distance_;  
};
#endif

#ifndef pid_H
#define pid_H

#define DEBUG 1

#include <ESP8266WiFi.h>

/**
 * 
 */
class PID {
  
  public:
    /**
     * 
     */
    PID();
    /**
     * 
     */

    PID(double _kp, double _ki, double _kd);
    /**
     * 
     */   
    void setTarget(double _target);
    /**
     * 
     */    
    void setInput(double _input);
    /**
     * 
     */
    void setTunings(double _kp, double _ki, double _kd);
    /**
     * 
     */
    void setOutputLimits(double _min, double _max);
     /**
     * 
     */
    double compute();
    /**
     * 
     */
    double getKp();
    /**
     * 
     */
    double getKi();
     /**
     * 
     */
    double getKd();
    /**
     * 
     */
    double getTarget();
     /**
     * 
     */
    double getInput();
    /**
     * 
     */
    void setActive(bool _active);
    
  private:
    void reset();
    double kp, ki, kd;
    double input, target, output;
    double ITotal, prevInput, prevError;
    double minLimit, maxLimit;
    bool active;  
};

#endif


#ifndef Robot_H
#define Robot_H

#include "Motor.h"
#include "Ultrasonic.h"

#define DEBUG 1

class Robot {
  public:
    /**
     * 
     */
    Robot();
    /**
     * 
     */
    void move(double velocity_x, double velocity_theta);
    /**
     * 
     */
    void stop();
    /**
     * 
     */
    void updateEncoder(int number);
    /**
     *
     */
    void updateControlLoopLowLevel();
    /**
     * 
     */
    void updateControlLoopHighLevel(double dt);
    /**
     * 
     */
    void updateDistance();
    /**
     *
     */
    double getX() { return x_; };
    /**
     *
     */
    double getY() { return y_; };
    /**
     *
     */
    double getTheta() { return theta_; };
    /**
     *
     */
    double getVx() { return vx_; };
    /**
     *
     */
    double getVy() { return vy_; };
    /**
     *
     */
    double getVtheta() { return vtheta_; };
     /**
     *
     */
    double getDistance();

  private:
    /**
     * 
     */
    Motor *motor_[2];
    /**
     * 
     */
    Ultrasonic  *ultrasonic;
    /**
     * 
     */
    double x_;
    /**
     * 
     */
    double y_;
    /**
     * 
     */
    double theta_;
    /**
     *
     */
    double vx_;
    /**
     *
     */
    double vy_;
    /**
     *
     */
    double vtheta_;
    /**
     *
     */
    const double wheel_separation_= 0.135;
    /**
     * 
     */
    const double wheel_radious_= 0.0325;
};

#endif



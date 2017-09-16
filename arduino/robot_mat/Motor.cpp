#include "Motor.h"

Motor::Motor(int pin_power,int pin_direction,int pin_position) : 
  encoder_(pin_power,pin_direction,pin_position) 
{
	angle_=0;
	previous_angle_=0;
}

void Motor::updateEncoder()
{
	encoder_.updateEncoder();
}

void Motor::move(double velocity)
{
  encoder_.move(velocity/angle_per_tic_);
}

void Motor::stop()
{
  encoder_.stop();
}

void Motor::updateControlLoopLowLevel()
{
	encoder_.updateControlLoop();
}

void Motor::updateControlLoopHighLevel()
{
  angle_=encoder_.getEncoder()*angle_per_tic_;
}

double Motor::getVelocity(double dt)
{
  double velocity  = (angle_ - previous_angle_)/dt;
  previous_angle_ = angle_;

  return velocity;
}

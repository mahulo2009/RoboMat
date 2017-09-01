#include "Motor.h"

Motor::Motor(int pin_power,int pin_direction,int pin_position) : 
  encoder_(pin_power,pin_direction,pin_position) 
{
	position_=0;
	previous_position_=0;

}

void Motor::updateEncoder()
{
	encoder_.updateEncoder();
}

void Motor::move(double velocity)
{
	encoder_.move(velocity/distance_per_count_);
}

void Motor::updateControlLoopLowLevel()
{
	encoder_.updateControlLoop();
}

void Motor::updateControlLoopHighLevel()
{
  position_=encoder_.getEncoder()*distance_per_count_;
}

double Motor::getPosition()
{
	return position_;
}

double Motor::getDistance()
{
  double distance  = (position_ - previous_position_);
  previous_position_ = position_;

  return distance;
}

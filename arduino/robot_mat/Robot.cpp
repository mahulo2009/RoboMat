#include "Robot.h"

Robot::Robot() {
	motor_[0] = new Motor(5,0,14);
	motor_[1] = new Motor(4,2,12);
  x_=0;
  y_=0;
  theta_=0;
  vx_=0;
  vy_=0;
  vtheta_=0;
}

void Robot::move(double velocity_x, double velocity_theta)
{
  double velocity_1  = 0;
  double velocity_2  = 0;
   
  if(velocity_x == 0)
  {
    velocity_1 = velocity_theta * wheel_separation_ / 2.0;
    velocity_2 = (-1) * velocity_1;
  }
  else if(velocity_theta == 0)
  {
    velocity_1 = velocity_x;
    velocity_2 = velocity_x;
  }
  else
  {
    velocity_1  = velocity_x - velocity_theta * wheel_separation_ / 2.0;
    velocity_2  = velocity_x + velocity_theta * wheel_separation_ / 2.0;
  }

/*  
  Serial.print(velocity_1);
  Serial.print("\t");
  Serial.print(velocity_2);
  Serial.print("\n");
*/
  motor_[0]->move(velocity_1);
  motor_[1]->move(velocity_2);
}

void Robot::updateEncoder(int number) {
  if ( (number <0) || (number >1) ) {
    return;
  }  
  motor_[number]->updateEncoder();
}

void Robot::updateControlLoopLowLevel() {
  for (int i=0;i<2;i++) {
	  motor_[i]->updateControlLoopLowLevel();
  }
}

void Robot::updateControlLoopHighLevel(double dt)
{
  for (int i=0;i<2;i++) {
    motor_[i]->updateControlLoopHighLevel();
  }

	double engine_distance_1  = motor_[0]->getDistance();
  double engine_distance_2  = motor_[1]->getDistance();

	vx_ = (engine_distance_1 + engine_distance_2) / 2.;
	vy_ = 0;

	theta_ = ((motor_[0]->getPosition() - motor_[1]->getPosition())/((TWO_PI * 0.035) / 20)) / 45. * PI;

	double delta_x = (vx_ * cos(theta_)) * dt;
	double delta_y = (vx_ * sin(theta_)) * dt;

	x_ += delta_x;
	y_ += delta_y;

	vtheta_ = theta_ / dt;


  Serial.print(theta_);
  Serial.print("\t");
  Serial.print(motor_[0]->getPosition());
  Serial.print("\t");
  Serial.print(motor_[1]->getPosition());
  Serial.print("\n");
}

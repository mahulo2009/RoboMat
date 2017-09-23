#include "Robot.h"

Robot::Robot() {
	motor_[0] = new Motor(5,0,14);
	motor_[1] = new Motor(4,2,12);
  ultrasonic = new Ultrasonic(16,15);
  x_=0;
  y_=0;
  theta_=0;
  vx_=0;
  vy_=0;
  vtheta_=0;
  theta_=0;
}

void Robot::move(double velocity_x, double velocity_theta)
{
  double velocity_1  = ( velocity_x + velocity_theta * wheel_separation_) / ( wheel_radious_ ) ;
  double velocity_2  = ( velocity_x - velocity_theta * wheel_separation_) / ( wheel_radious_ ) ;
/*
  Serial.print("move velocity:");
  Serial.print("\t");
  Serial.print(velocity_1);
  Serial.print("\t");
  Serial.print(velocity_2);
  Serial.print("\n");
*/
  motor_[0]->move(velocity_1);
  motor_[1]->move(velocity_2);
}

void Robot::stop()
{  
  for (int i=0;i<2;i++) {
   motor_[i]->stop();
  }
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

	double velocity_1  = motor_[0]->getVelocity(dt);
  double velocity_2  = motor_[1]->getVelocity(dt);

	vx_ = ( wheel_radious_ * ( velocity_1 + velocity_2 ) ) / 2.;
	vy_ = 0;
  vtheta_ = ( ( wheel_radious_ * ( velocity_1 - velocity_2 ) ) /  ( wheel_separation_ ) ) ;
 
	x_ +=  vx_ * cos(theta_) * dt;
	y_ +=  vx_ * sin(theta_) * dt;
  theta_+= vtheta_ * dt;
  
  /*
	Serial.print("updateControlLoopHighLevel velocity:");
  Serial.print("\t");
  Serial.print(velocity_1);
  Serial.print("\t");
  Serial.print(velocity_2);
  Serial.print("\t");
  Serial.print(vx_);
  Serial.print("\t");
  Serial.print(x_);
  Serial.print("\t");
  Serial.print(y_);
  Serial.print("\t");
  Serial.print(theta_);
  Serial.print("\n"); 
  */
}

void Robot::updateDistance()
{
  ultrasonic->updateDistance();
}

double Robot::getDistance()
{
   return ultrasonic->distance()/100; 
}



#include "pid.h"

PID::PID(double _kp, double _ki, double _kd) {
  kp = _kp;
  ki = _ki;
  kd = _kd;
  reset();
  output = 0;
  input = 0;
    
  setOutputLimits(0, 1023);
}

PID::PID() {
  reset();
}

void PID::setTarget(double _target) {
  target = _target;
}

void PID::setInput(double _input) {
  input = _input;
}

void PID::setTunings(double _kp, double _ki, double _kd) {
  kp = _kp;
  ki = _ki;
  kd = _kd;
    
  reset();
};

void PID::setOutputLimits(double _min, double _max) {
  if (_min > _max) return;
    
   minLimit = _min;
   maxLimit = _max;
}

double PID::compute() {
  long now = millis();
  long change = now - prevTime;
  
  double error = target - input;
  ITotal += error * ki;
  ITotal = min(ITotal, maxLimit);
  ITotal = max(ITotal, minLimit);    
  double dValue = kd * (error - prevError)/(change/1000.0);
  
  /* do the full calculation */
  output = kp * error + ITotal + dValue;
    
  /* clamp output to bounds */
  output = min(output, maxLimit);
  output = max(output, minLimit);  
  
  /* required values for next round */
  prevTime = now;
  prevInput = input;
    
  /* debug some PID settings */
  
  return output;
};

double PID::getKp() {
  return kp;
}

double PID::getKi() {
  return ki;
}

double PID::getKd() {
  return kd;
}

double PID::getTarget() {
    return target;
}

double PID::getInput() {
    return input;
}

void PID::setActive(bool _active) {
  if (!active && _active)
    reset();
  active = _active;
};

void PID::reset() {
  ITotal = output;
  prevInput = 0;
  prevError = 0;
    
  ITotal = min(ITotal, maxLimit);
  ITotal = max(ITotal, minLimit); 
};


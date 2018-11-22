#include "DCMotor.hpp"

void DCMotor::attach(int pin) {
  this->_motor.attach(pin);
}
void DCMotor::detach() {
  this->_motor.detach();
}

void DCMotor::set(int value) {
  int _val = value;
  if (this->_reversed) _val = -1 * _val;
  this->_motor.write(map(_val, -127, 127, 0, 180));
}

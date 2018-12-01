#include "Chassis.hpp"

void Chassis::attachAll() {
  this->_left.attach(this->_leftPin);
  this->_right.attach(this->_rightPin);
}
void Chassis::detachAll() {
  this->_left.detach();
  this->_right.detach();
}

void Chassis::drive(int power, int duration) {
  this->_left.set(power);
  this->_right.set(power);
  delay(duration);
}

void Chassis::turnCW(int power, int duration) {
  this->_left.set(80);
  this->_right.set(-80);
  delay(duration);
}

void Chassis::turnCCW(int power, int duration) {
  this->_left.set(0 - power);
  this->_right.set(power);
  delay(duration);
}

void Chassis::stop() {
  this->_left.set(0);
  this->_right.set(0);
}

#include "Chassis.hpp"

void Chassis::attachAll() {
  this->_left.attach(this->_leftPin);
  this->_right.attach(this->_rightPin);
}
void Chassis::detachAll() {
  this->_left.detach();
  this->_right.detach();
}

void Chassis::drive(int lp, int rp, int duration) {
  this->_left.set(lp);
  this->_right.set(rp);
  delay(duration);
}

void Chassis::stop() {
  this->_left.set(0);
  this->_right.set(0);
}

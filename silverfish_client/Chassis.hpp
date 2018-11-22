#ifndef _CHASSIS_HPP_
#define _CHASSIS_HPP_

#include <Arduino.h>
#include "DCMotor.hpp"

class Chassis {
  private:
    DCMotor _left;
    DCMotor _right;

    int _leftPin;
    int _rightPin;
  public:
    Chassis(int leftPin, int rightPin) : _left(), _right(true), _leftPin(leftPin), _rightPin(rightPin) {};
    void attachAll();
    void detachAll();

    void forward(int power, int duration);
    void reverse(int power, int duration);
    void turnCW(int power, int duration);
    void turnCCW(int power, int duration);

    void stop();
};

#endif

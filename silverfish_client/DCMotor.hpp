#ifndef _DC_MOTOR_HPP
#define _DC_MOTOR_HPP

#include <Arduino.h>
#include <ESP32_Servo.h>

class DCMotor {
  private:
    Servo _motor;
    bool _reversed;
  public:
    DCMotor(bool reversed = false) : _motor(Servo()), _reversed(reversed) {};
    void attach(int pin);
    void detach();
    void set(int value);
};

#endif

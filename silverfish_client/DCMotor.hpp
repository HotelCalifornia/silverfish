#ifndef _DC_MOTOR_HPP
#define _DC_MOTOR_HPP

#include <Arduino.h>
#include <ESP32_Servo.h>

class DCMotor {
  private:
    Servo _motor;
    bool _reversed;
  public:
    /**
     * @param [reversed=false] whether to negate values sent to this motor
     */
    DCMotor(bool reversed = false) : _motor(Servo()), _reversed(reversed) {};
    /**
     * Attach the internal servo instance to the pin from which it can be controlled
     */
    void attach(int pin);
    /**
     * Detach the internal servo instance from its pin, freeing the pin for other uses
     */
    void detach();
    /**
     * Send a power value to the motor
     * 
     * @param value the power to be sent
     */
    void set(int value);
};

#endif

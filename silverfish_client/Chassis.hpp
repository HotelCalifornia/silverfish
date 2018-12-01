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
    /**
     * Construct a chassis with one motor each on the left and right
     * 
     * @param leftPin the pin from which the left-side motor can be controlled
     * @param rightPin the pin from which the right-side motor can be controlled
     */
    Chassis(int leftPin, int rightPin) : _left(true), _right(true), _leftPin(leftPin), _rightPin(rightPin) {};
    /**
     * Attach motor servo instances to the pins given to the constructor
     * 
     * This should be called from setup()
     */
    void attachAll();
    /**
     * Detach motor servo instances from their pins, freeing them for use by other servomotors
     */
    void detachAll();

    /**
     * Drive
     * 
     * positive values drive forward, negative ones drive in reverse
     * 
     * @param power the power to send to the motors
     * @param duration how long in milliseconds to drive, blocking until this has elapsed
     */
    void drive(int power, int duration);
    
    /**
     * Turn clockwise
     * 
     * @param power the power to send to the motors
     * @param duration how long in milliseconds to drive, blocking until this has elapsed
     */
    void turnCW(int power, int duration);
    /**
     * Turn counterclockwise
     * 
     * @param power the power to send to the motors
     * @param duration how long in milliseconds to drive, blocking until this has elapsed
     */
    void turnCCW(int power, int duration);

    /**
     * Stop the chassis
     */
    void stop();
};

#endif

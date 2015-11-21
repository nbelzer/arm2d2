/*
    ServoController.h - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "Adafruit_PWMServoDriver.h"

class ServoController
{
    public:
        ServoController(int servoAmount);
        void MoveServo(int servoId, int toState);
        void MoveServoOverTime(int *servoId, int toState, int inMillis, int servos = 1);
        void ResetServos();
        
        int amountOfServos;
    private:
        int StateToPulse(int servoId, int state);
        
        Adafruit_PWMServoDriver _pwm;
};

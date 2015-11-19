/*
    ServoController.h - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "Adafruit_PWMServoDriver.h"

class ServoController
{
    public:
        ServoController(void);
        void MoveServo(int servoId, int toState);
        void MoveServoOverTime(int *servoId, int toState, int inMillis, int servos = 1);
        void ResetServos();
    private:
        int rotationToPulse(int servoId, int state);
        Adafruit_PWMServoDriver _pwm;
};

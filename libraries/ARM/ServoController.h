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
        void MoveServo(int servoId, int toDegrees);
        // void MoveServos(int servos[], int toDegrees);
    private:
        int rotationToPulse(int servoId, int rotation);
        Adafruit_PWMServoDriver _pwm;
};

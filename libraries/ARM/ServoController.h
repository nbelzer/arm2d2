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
        void MoveServoOverTime(int servoId, int toState, int inMillis);
        void MoveServosOverTime(int *servoId, int toState, int inMillis, int servos = 1);
        void ResetServos();
        void SoftReset();
        
        /* All the servos that the ARM has */
        int baseRotServo = 0, 
            armServo = 3, 
            armRotServo = 4, 
            handRotServo = 5, 
            handServo = 6;
        int bodyServos[2] = {1, 2};
    private:
        int StateToPulse(int servoId, int state);
        
        Adafruit_PWMServoDriver _pwm;
};

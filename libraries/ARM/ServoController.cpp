/*
    ServoController.cpp - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "ServoController.h"
#include "Adafruit_PWMServoDriver.h"

#define NELEMENTS(x) (sizeof(x)/sizeof((x)[0]));

/* parameters */

int amountOfServos = 7;
int servoState[] = { 0,0,0,0,0,0,0 };
int _minServo[] = { 210,190,190,210,300,300,450 };
int _maxServo[] = { 590,480,480,590,550,550,650 };

Adafruit_PWMServoDriver _pwm = Adafruit_PWMServoDriver();

/* constructor */

ServoController::ServoController(void)
{
    _pwm.begin();
    _pwm.setPWMFreq(60);
}

/* public methods */

void ServoController::MoveServo(int servoId, int toState)
{
    _pwm.setPWM(servoId, 0, rotationToPulse(servoId, toState));
    servoState[servoId] = toState;
}

void ServoController::MoveServoOverTime(int *servoId, int toState, int inMillis, int servos)
{
    int millisPassed = 0;
    inMillis = max(inMillis, 400); // A fail safe for the servo, moving faster than this can cause problems.
    int lastMillis = millis();
    int amount = servos;
    int fromState[amount];
    int deltaState[amount];
    
    for (int i = 0; i < amount; i++)
    {
        fromState[i] = servoState[servoId[i]];
        deltaState[i] = toState - fromState[i];
    }
    
    while (millisPassed < inMillis)
    {
        int deltaMillis = millis() - lastMillis;
        lastMillis = millis();
        millisPassed += deltaMillis;
        
        float percentage = ((float) millisPassed) / inMillis;
        
        for (int i = 0; i < amount; i++)
        {
            float newState = percentage * deltaState[i] + fromState[i];
            MoveServo(servoId[i], newState);
        }
    }
}

/* private methods */

/// <summary>
/// This method resets the servos, for testing purposes it first of all sets them to their minimum 
/// and after a delay of 10 seconds it sets them to their maximum.
/// </summary>
void ServoController::ResetServos(void)
{
    for (int s = 0; s < amountOfServos; s++)
        MoveServo(s, 0);
        
    // delay(10000);
        
    // for (int s = 0; s < amountOfServos; s++)
    //     MoveServo(s, 1);
}

/* rotationToPulse returns a pulse value according to a rotatin value from 0-1 */
int ServoController::rotationToPulse(int servoId, int state)
{
    return map(state, 0, 100, _minServo[servoId], _maxServo[servoId]);
}

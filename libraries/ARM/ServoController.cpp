/*
    ServoController.cpp - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "ServoController.h"
#include "Adafruit_PWMServoDriver.h"

int servoState[] = { 0,0,0,0,0,0,0 };
int _minServo[] = { 210,210,210,210,210,210,0 };
int _maxServo[] = { 590,590,590,590,590,590 };

Adafruit_PWMServoDriver _pwm = Adafruit_PWMServoDriver();

ServoController::ServoController(void)
{
    _pwm.begin();
    _pwm.setPWMFreq(60);
}

void ServoController::MoveServo(int servoId, int toRotation)
{
    Serial.println("Moving");
    _pwm.setPWM(servoId, 0, rotationToPulse(servoId, toRotation));
    servoState[servoId] = toRotation;
}

/* rotationToPulse returns a pulse value according to a rotatin value from 0-1 */
int ServoController::rotationToPulse(int servoId, int rotation)
{
    return map(rotation, 0, 180, _minServo[servoId], _maxServo[servoId]);
}

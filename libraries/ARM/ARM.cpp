/*
    ARM2D2.cpp - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "ARM.h";

ARM::ARM(void)
{
    controller = ServoController();
}

void ARM::RunTest(void)
{
    controller.ResetServos();
    
    delay(1000);
    
    int bodyServos[2] = {1, 2};
    controller.MoveServoOverTime(bodyServos, 40, 1000, 2);
    
    int servo[1] = {4};
    controller.MoveServoOverTime(servo, 80, 1000);
    servo[0] = 5;
    controller.MoveServoOverTime(servo, 80, 1000);
}

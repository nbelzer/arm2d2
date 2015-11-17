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
    while(true)
    {
        controller.MoveServo(0, 40);
        delay(100);
        controller.MoveServo(1, 40);
        controller.MoveServo(2, 40);
        delay(2000);

        controller.MoveServo(0, 150);
        delay(100);
        controller.MoveServo(1, 150);
        controller.MoveServo(2, 150);
        delay(2000);
    }
}

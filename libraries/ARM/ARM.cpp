/*
    ARM2D2.cpp - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "ARM.h";

ARM::ARM(void)
{
    controller = ServoController();
    controller.ResetServos();
}

void ARM::RunTest(void)
{
    
}

void ARM::PickUpItemRoutine(void)
{
    // Defining all the servos
    int handServo[1] = {6};
    int armServo[1] = {3};
    int armRotServo[1] = {4};
    int bodyServos[2] = {1, 2};
    int rotationServo[1] = {0};
    
    // Quick setup
    controller.MoveServo(3, 30);
    controller.MoveServo(4, 80);
    
    while (true)
    {
        // Grabbing the item
        controller.MoveServoOverTime(handServo, 0, 2000, 1);
        delay(5000);
        controller.MoveServoOverTime(handServo, 100, 4000, 1);
        
        // Actual action
        controller.MoveServoOverTime(armServo, 50, 2000, 1);
        controller.MoveServoOverTime(bodyServos, 40, 4000, 2);
        controller.MoveServoOverTime(armServo, 80, 2000, 1);
        controller.MoveServoOverTime(rotationServo, 50, 2000, 1);
        controller.MoveServoOverTime(armRotServo, 95, 1000, 1);
        
        // Release the object
        controller.MoveServoOverTime(handServo, 0, 4000, 1);
        
        // Reset servos
        controller.MoveServoOverTime(armRotServo, 80, 1000, 1);
        controller.MoveServoOverTime(rotationServo, 0, 2000, 1);
        controller.MoveServoOverTime(armServo, 50, 2000, 1);
        controller.MoveServoOverTime(bodyServos, 0, 2000, 2);
        controller.MoveServoOverTime(armServo, 30, 2000, 1);
    }
}

#include <Wire.h>

/*
    Arm2D2.ino - ARM2D2 Project
    Created by Nick Belzer, November 13, 2015
*/

#include <ARM.h>

/* Setup all the items */
void setup()
{
    ARM arm = ARM();
    arm.CommandMode();
    
    // while (true)
    // {
    //     arm.controller.MoveServoOverTime(3, 30, 1000);
    //     delay(500);
    //     arm.controller.MoveServoOverTime(3, 10, 1000);
    //     delay(500);
    // }
}

/* Run the loop */
void loop()
{ 
}

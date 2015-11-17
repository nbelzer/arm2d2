/*
    Arm2D2.ino - ARM2D2 Project
    Created by Nick Belzer, November 13, 2015
*/

#include <ARM.h>
// #include <ServoController.h>

/* Setup all the items */
void setup()
{
    Serial.begin(9600);
    Serial.println("Hi");
    ARM arm = ARM();
    arm.RunTest();
}

/* Run the loop */
void loop()
{
}

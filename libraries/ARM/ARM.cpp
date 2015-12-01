/*
    ARM2D2.cpp - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "ARM.h";

/* All the servos that the ARM has */
// int baseRotServo = 0;
// int bodyServos[] = {1, 2};
// int armServo = 3;
// int armRotServo = 4;
// int handRotServo = 5;
// int handServo = 6;

ARM::ARM(void)
{
    controller = ServoController();
    controller.ResetServos();
    reader = DataReader();
}

void ARM::CommandMode(void)
{
	Serial.write("ARM2-D2 at your service...\n");
    Serial.write("\nEntered command mode, use commands to control the ARM,\nUse 'ARM help' to get more information about the commands.");
    bool running = true;
    while(running)
    {
        delay(200);
        /* If some command was entered incorrectly it could get the entire program to be stuck, therefore we must clean the stored data */
        reader.CleanStored();
        
        if (reader.ReadCommand("ARM pickup") == 0)
        {
            Serial.write("\nPicking up item routine  ");
            PickUpItemRoutine();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM reset") == 0)
        {
            Serial.write("\nSetting all servos to 0  ");
            controller.SoftReset();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM open") == 0)
        {
            Serial.write("\nOpening the claw         ");
            controller.MoveServoOverTime(controller.handServo, 0, 500);
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM close") == 0)
        {
            Serial.write("\nClosing the claw         ");
            controller.MoveServoOverTime(controller.handServo, 100, 500);
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM standup") == 0)
        {
            Serial.write("\nPosing for the camera    ");
            controller.MoveServoOverTime(controller.armServo, 30, 1000);
            controller.MoveServoOverTime(controller.armRotServo, 80, 1000);
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM stop") == 0)
        {
            Serial.write("\nARM2-D2 shutting down    ");
            controller.SoftReset();
            Serial.write("\t[DONE]");
            Serial.write("\n\nSee you later");
            running = false;
        }
        if (reader.ReadCommand("ARM move") == 0)
        {
            bool waiting = true;
            
            Serial.write("\nMoving ServoId: ");
            int servoId, state, time;
            while (waiting)
            {
               while (Serial.available() == 0);
               servoId = Serial.parseInt(); 
               if (servoId >= 0 && servoId < 16) waiting = false; 
            }
            Serial.print(servoId);
            
            waiting = true;
            Serial.write(" State [0...100]: ");
            while(waiting)
            {
                while(Serial.available() == 0);
                state = Serial.parseInt();
                if (state >= 0 && state <= 100) waiting = false;
            }
            Serial.print(state);
            
            waiting = true;
            Serial.write(" Time [0...60000]: ");
            while (waiting)
            {
                while(Serial.available() == 0);
                time = Serial.parseInt();
                if (time >= 0 && time <= 60000) waiting = false;
            }
            Serial.print(time);

            /* Exception for servo 1 and 2, they need to move together and thus when entering 1 or 2 they should both move. */
            if (servoId == 1 || servoId == 2) controller.MoveServosOverTime(controller.bodyServos, state, time, 2);
            else controller.MoveServoOverTime(servoId, state, time);

            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM help") == 0)
        {
            Serial.println("\n\nARM2-D2 Command Mode");
            Serial.println("\nPossible commands:");
            Serial.println("\tARM move\t\tA command to make a specific servo move.");
            Serial.println("\tARM reset\t\tResets all the servos to the 0 position.");
            Serial.println("\tARM open\t\tOpens the claw.");
            Serial.println("\tARM close\t\tCloses the claw.");
            Serial.println("\tARM standup\tMoves the arm to a 'standup' position.");
            Serial.println("\tARM pickup\t\tA command to start the 'pick up item' routine.");
            Serial.println("\tARM stop\t\tResets all the servos to 0 and stops the program.");
            
            Serial.print("\n");
        }
    }
}

void ARM::PickUpItemRoutine(void)
{   
    // Moving the arm to start position
    controller.MoveServoOverTime(controller.armServo, 30, 1000);
    controller.MoveServoOverTime(controller.armRotServo, 80, 1000);
    delay(4000);;
    
    // Grabbing the item
    controller.MoveServoOverTime(controller.handServo, 0, 1000);
    delay(1000);
    controller.MoveServoOverTime(controller.handServo, 100, 1000);
    
    // Actual action
    controller.MoveServoOverTime(controller.armServo, 50, 1000);
    controller.MoveServosOverTime(controller.bodyServos, 40, 1000, 2);
    controller.MoveServoOverTime(controller.armServo, 80, 1000);
    controller.MoveServoOverTime(controller.baseRotServo, 50, 1000);
    controller.MoveServoOverTime(controller.armRotServo, 95, 500);
    
    // Release the object
    controller.MoveServoOverTime(controller.handServo, 0, 2000);
    
    // Reset servos
    controller.MoveServoOverTime(controller.armRotServo, 80, 500);
    controller.MoveServoOverTime(controller.baseRotServo, 0, 1000);
    controller.MoveServoOverTime(controller.armServo, 50, 1000);
    controller.MoveServosOverTime(controller.bodyServos, 0, 1000, 2);
    controller.MoveServoOverTime(controller.armServo, 30, 1000);
}

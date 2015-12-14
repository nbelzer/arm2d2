/*
    ARM2D2.cpp - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "ARM.h";

/** Create the variables we need later on */
ARM::ARM(void)
{
    controller = ServoController();
    controller.ResetServos();
    reader = DataReader();
}
/** CommandMode
    Starts a loop that wil only end when giving a specific command. In this loop you will be able to give commands via the serial monitor included with the Arduino program.
*/

void ARM::CommandMode(void)
{
	Serial.write("ARM2-D2 at your service...\n");
    Serial.write("\nEntered command mode, use commands to control the ARM.");
    bool running = true;
    while(running)
    {
        delay(200);
        /* If some command was entered incorrectly it could get the entire program to be stuck, therefore we must clean the stored data */
        reader.CleanStored();
        
        if (reader.ReadCommand("ARM.pickup") == 0)
        {
            Serial.write("\nPicking up item routine");
            PickUpItemRoutine();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.reset") == 0)
        {
            Serial.write("\nSetting all servos to 0");
            controller.SoftReset();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.open") == 0)
        {
            Serial.write("\nOpening the claw");
            controller.MoveServoOverTime(controller.handServo, 0, 500);
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.close") == 0)
        {
            Serial.write("\nClosing the claw");
            controller.MoveServoOverTime(controller.handServo, 100, 500);
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.standup") == 0)
        {
            Serial.write("\nPosing for the camera");
            Standup();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.stop") == 0)
        {
            Serial.write("\nARM2-D2 shutting down");
            controller.SoftReset();
            Serial.write("\t[DONE]");
            Serial.write("\n\nSee you later");
            running = false;
        }
        if (reader.ReadCommand("ARM.throw") == 0)
        {
            Serial.write("\nGive me the item");
            ThrowAway();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.forward") == 0)
        {
            Serial.write("\nMoving face forward");
            ForwardFacing();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.hello") == 0)
        {
            Serial.write("\nHello!");
            SayHello();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.shake") == 0)
        {
            Serial.write("\nHandShake!");
            HandShake();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.random") == 0)
        {
            Serial.write("\nBe prepared!");
            RandomMoveARM();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.rps") == 0)
        {
            RockPaperScissors();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.golf") == 0)
        {    
            Serial.write("\nI love golfing           ");
            Golf();
            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM.move") == 0)
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
            if (servoId == 1 || servoId == 2) 
            {
                int positions[2] = { state, state };
                controller.MoveServosOverTime(controller.bodyServos, positions, time, 2);
            }
            else controller.MoveServoOverTime(servoId, state, time);

            Serial.write("\t[DONE]");
        }
        if (reader.ReadCommand("ARM help") == 0)
        {
            Serial.println("\nDue to limitations in the Arduino hardware it is not possible to upload the entire command list here, please look at the documentation");
            // Serial.println("\n\nARM2-D2 Command Mode");
            // Serial.println("\nPossible commands:");
            // Serial.println("\tARM.move\t\tA command to make a specific servo move.");
            // Serial.println("\tARM.reset\t\tResets all the servos to the 0 position.");
            // Serial.println("\tARM.open\t\tOpens the claw.");
            // Serial.println("\tARM.close\t\tCloses the claw.");
            // Serial.println("\tARM.standup\tMoves the arm to a 'standup' position.");
            // Serial.println("\tARM.pickup\t\tA command to start the 'pick up item' routine.");
            // Serial.println("\tARM.stop\t\tResets all the servos to 0 and stops the program.");
        }
    }
}

/** PickupItemRoutine
    Picks up an item and places it behind it, the start position is the poition the arm is in at the moment of entering this command.
*/
void ARM::PickUpItemRoutine(void)
{   
    OpenClaw();
    delay(4000);
    CloseClaw();
    
   // Servo pair
    int servos[3] = { 1, 2, controller.armServo };
    
    int startServos[2] = { controller.armServo, controller.armRotServo };
    int startPositions[2] = { 30, 20 };
    // Moving the arm to start position
    controller.MoveServosOverTime(startServos, startPositions, 1000, 2);
    
    // Actual action
    int positions[3] = { 30, 30, 80 };
    controller.MoveServosOverTime(servos, positions, 1500, 3);
    controller.MoveServoOverTime(controller.baseRotServo, 100, 2500);
    
    // Release the object
    ForwardFacing();
    OpenClaw();
    
    // Reset servos
    controller.SoftReset();
    OpenClaw();
}

/** Opens the claw */
void ARM::OpenClaw(void)
{
    controller.MoveServoOverTime(controller.handServo, 0, 500);
}

/** Closes the claw */
void ARM::CloseClaw(void)
{
    controller.MoveServoOverTime(controller.handServo, 100, 500);
}

/** Moves the arm to a 'standup' position */
void ARM::Standup(void)
{   
    int startServos[2] = { controller.armServo, controller.armRotServo };
    int startPositions[2] = { 30, 80 };
    
    // Moving the arm to start position
    controller.MoveServosOverTime(startServos, startPositions, 1000, 2);
    delay(4000);; 
}

/** Throw's away an item, usefull for throwing a dice */
void ARM::ThrowAway(void)
{
    controller.SoftReset();
    Standup();
    // pickup the item
    OpenClaw();
    // wait for someone to give the item
    delay(4000);
    CloseClaw();
    
    int servos[5] = { 1, 2, controller.armServo, controller.armRotServo, controller.handServo };
    int positions[5] { 0, 0, 40, 40, 100};
    
    // Fall back a bit
    controller.MoveServosOverTime(servos, positions, 2000, 5);
    
    positions[0] = 40; 
    positions[1] = 40; 
    positions[2] = 50; 
    positions[3] = 50; 
    positions[4] = 0;
    
    // Throw the item
    controller.MoveServosOverTime(servos, positions, 600, 5);
}

/** The arm waves */
void ARM::SayHello(void)
{
    controller.SoftReset();
    Standup();
    controller.MoveServoOverTime(controller.handRotServo, 100, 500);
    controller.MoveServoOverTime(controller.armServo, 30, 1000);
    controller.MoveServoOverTime(controller.armRotServo, 30, 1000);
    
    int servos[3] = { controller.armServo, controller.handServo, controller.armRotServo };
    int sPos[3] = { 10, 100, 60 };
    int ePos[3] = { 40, 0, 30 };
    for (int i=0; i < 3; i++)
    {
        controller.MoveServosOverTime(servos, sPos, 600, 3);
        delay(100);
        controller.MoveServosOverTime(servos, ePos, 600, 3);
        delay(100);
    }
    
    controller.SoftReset();
}

/** The arm gives a handshake, not really great but it kind of works, the arm itself is not strong enough for this to be possible */
void ARM::HandShake(void)
{
    controller.SoftReset();
    Standup();
    CloseClaw();
    controller.MoveServoOverTime(controller.handRotServo, 100, 500);
    controller.MoveServoOverTime(controller.armServo, 30, 1000);
    controller.MoveServoOverTime(controller.armRotServo, 30, 1000);
    
    int servos[3] = { controller.armServo, controller.armRotServo };
    int sPos[2] = { 10, 60 };
    int ePos[2] = { 50, 90 };
    for (int i=0; i < 2; i++)
    {
        controller.MoveServosOverTime(servos, sPos, 800, 2);
        delay(100);
        controller.MoveServosOverTime(servos, ePos, 900, 2);
        delay(100);
    }
    
    delay(1500);
    controller.SoftReset();
}

/** Moves the arm so that the claw is just above the table */
void ARM::ForwardFacing(void)
{
    Standup();
    
    int servos[4] = { 1, 2, controller.armServo, controller.armRotServo };
    int positions[4] = { 60, 70, 50, 15 };
    
    controller.MoveServosOverTime(servos, positions, 2000, 4);
    OpenClaw();
}

// mode 0 is low-power, mode 5 is high power;
/** Get's the arm in golf mode where you are able to enter in the strenght of the hit */
void ARM::Golf()
{
    controller.SoftReset();
    ForwardFacing();
    CloseClaw();
    controller.MoveServoOverTime(controller.baseRotServo, 20, 1000);
    Serial.write("\nHow hard should I hit [0..5]? ");   
            
    bool waiting = true;
    int mode = 0;
    while (waiting)
    {
        while(Serial.available() == 0);
        mode = Serial.parseInt();
        if (mode >= 0 && mode <= 5) waiting = false;
    }
    Serial.print(mode);
    
    int time = (5-mode) * 400;
    controller.MoveServoOverTime(controller.baseRotServo, 0, time);
}

/** Keeps moving the arm to a random position, nice for showing of */
void ARM::RandomMoveARM(void)
{
    int arm[4] = { 3, 4, 5, 6 };
    int positions[4] = { 0, 0, 0 ,0 };
    int basepos[2] = { 20, 20 };
    controller.MoveServosOverTime(controller.bodyServos, basepos, 1000, 2);
    bool running = true;
    Serial.print("\nRandomly switching between positions, \n
    give the command 'exit' to exit the loop");
    while(running)
    {
        delay(2000);
        /* If some command was entered incorrectly it could get the entire program to be stuck, therefore we must clean the stored data */
        reader.CleanStored();
        for (int i=0; i<4; i++)
        {
            positions[i] = rand() % 100;
            if (i == 0) positions[i] = max(positions[i], 25);
        }  
        controller.MoveServosOverTime(arm, positions, 3000, 4);
        if (reader.ReadCommand("exit")==0)
        {
            running = false;
        }
    }
    controller.SoftReset();
}

/** Plays the game Rock Paper Scissors */
void ARM::RockPaperScissors(void)
{
    Standup();
    bool running = true;
    int wins = 0; int loses = 0;
    Serial.write("\nWelcome to Rock Paper Scissors");
    Serial.write("\nUse the command 'go' to start a game,");
    Serial.write("\nthe command 'score' to see the score");
    Serial.write("\nor use the command 'exit' to exit");
    while(running)
    {
        delay(200);
        /* If some command was entered incorrectly it could get the entire program to be stuck, therefore we must clean the stored data */
        reader.CleanStored();
        
        if (reader.ReadCommand("go")==0)
        {
            Standup();
            Serial.write("\nHere we go ");
            int rps = rand() % 3; //0-2
            {
                int servos[2] = { controller.armServo, controller.armRotServo };
                int sPositions[2] = { 10, 30 };
                int ePositions[2] = { 30, 50 };
                for (int i=0; i<3; i++)
                {
                    Serial.print(3-i); Serial.print(" ");
                    controller.MoveServosOverTime(servos, ePositions, 400, 2);
                    controller.MoveServosOverTime(servos, sPositions, 400, 2);
                }
            }
            {
                int handServos[2] = { controller.handRotServo, controller.handServo };
                int rPositions[2] = { 0, 100 };
                int pPositions[2] = { 0, 0 };
                int sPositions[2] = { 100, 0 };
                int* positions;
                
                switch (rps)
                {
                    case 0: 
                        positions = rPositions;
                        Serial.write("\nRock!");
                        break;
                    case 1: 
                        positions = sPositions;
                        Serial.write("\nScissors!");
                        break;
                    case 2:
                        positions = pPositions;
                        Serial.write("\nPaper!");
                        break;
                }
                
                controller.MoveServosOverTime(handServos, positions, 800, 2);
            }
            
            bool waiting = true;
            Serial.write("\nDid I win [y/t/n]?");
            while (waiting)
            {
                delay(200);
                /* If some command was entered incorrectly it could get the entire program to be stuck, therefore we must clean the stored data */
                reader.CleanStored();
                
                if (reader.ReadCommand("y")==0)
                {
                    Serial.write("\nI won!");
                    waiting = false;
                    wins++;
                }
                if (reader.ReadCommand("t")==0)
                {
                    Serial.write("\nIt's a tie!");
                    waiting = false;
                }
                if (reader.ReadCommand("n")==0)
                {
                    Serial.write("\nI lost :(");
                    waiting = false;
                    loses++;
                }
            }
            int handServos[2] = { controller.handRotServo, controller.handServo };
            int endPositions[2] = { 0, 0 };
            controller.MoveServosOverTime(handServos, endPositions, 600, 2);
            
            Serial.write("\nCurrent score: ARM2-D2: "); 
            Serial.print(wins);
            Serial.write(" You: ");
            Serial.print(loses);
            Serial.write("\nWrite 'go' for another round or 'exit' to exit the game");
        }
        
        if (reader.ReadCommand("score")==0)
        {
            Serial.write("\nCurrent score: ARM2-D2: "); 
            Serial.print(wins);
            Serial.write(" You: ");
            Serial.print(loses);
        }
        if (reader.ReadCommand("exit")==0)
        {
            controller.SoftReset();
            running = false;
        }
    }
}
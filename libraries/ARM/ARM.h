/*
    ARM2D2.h - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "ServoController.h"
#include "DataReader.h"

class ARM
{
    public:
        ARM(void);
        void CommandMode(void);
        void PickUpItemRoutine(void);
        void OpenClaw(void);
        void CloseClaw(void);
        void Standup(void);
        void ThrowAway(void);
        void SayHello(void);
        void ForwardFacing(void);
        void Golf();
        void RockPaperScissors(void);
        
        ServoController controller;
        DataReader reader;
    private:
};

/*
    ARM2D2.h - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "ServoController.h"

class ARM
{
    public:
        ARM(void);
        void RunTest(void);
        ServoController controller;
    private:
};

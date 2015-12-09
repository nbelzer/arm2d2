String incomingMessage = "";

#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm;

void setup()
{
    Serial.begin(9600);
    pwm = Adafruit_PWMServoDriver();
    pwm.begin();
    pwm.setPWMFreq(60);
    pwm.setPWM(0, 0, 200);
}

void loop()
{
    if (Serial.available() > 0)
    {
        incomingMessage = Serial.readStringUntil(' ');

        Serial.print("I received: ");
        Serial.println(incomingMessage);
    }
}

void RunCommandFromInput()
{
    // The first element should contain the main command, like 'move' or 'reset'
}

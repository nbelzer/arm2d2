/**
    ServoController.cpp - ARM2D2 Library
    Created by Nick Belzer, November 13, 2015
*/

#include "Arduino.h"
#include "ServoController.h"
#include "Adafruit_PWMServoDriver.h"

#define NELEMENTS(x) (sizeof(x)/sizeof((x)[0]));

int amountOfServos = 7;
/* The current state of each servo, this starts at zero if we reset all the servos before using them. */
int servoState[] = { 0,0,0,0,0,0,0 };
/* The pulse length to give to a servo if the state is 0 */
int _minServo[] = { 210,190,190,210,300,300,450 };
/* The pulse length to give to a servo if the state is 100 */
int _maxServo[] = { 590,600,600,590,550,550,650 };



Adafruit_PWMServoDriver _pwm = Adafruit_PWMServoDriver();

/** ServoController
    Constructs the servo controller class, initialized the _pwm variable to use later on.
    
    @param servoAmount
    The amount of servos that you have connected and want to controll.
*/
ServoController::ServoController()
{
    _pwm.begin();
    _pwm.setPWMFreq(60);
}

/* PUBLIC FUNCTIONS */

/** MoveServo
    Give the signal to rotate a servo to a specific state.
    
    @param servoId [0 .. 16]
    The id of the port you connected the servo to.
    
    @param toState [0 .. 100]
    The state you want the servo to be in, every servo has a minimum and maximum value of rotation, this is different for each servo so every servo can be set to a number between 0 and 100.
*/
void ServoController::MoveServo(int servoId, int toState)
{
    _pwm.setPWM(servoId, 0, StateToPulse(servoId, toState));
    servoState[servoId] = toState;
}

/** MoveServoOverTime
    Basicly a wrapper around MoveServosOverTime that makes it easy to just enter in one number instead of having to make an array first.
*/
void ServoController::MoveServoOverTime(int servoId, int toState, int inMillis)
{
    int servo[1] = { servoId };
    int position[1] = { toState }; 
    MoveServosOverTime(servo, position, inMillis, 1);
}

/** MoveServoOverTime
    Rotates a servo over time, it uses the MoveServo method to rotate the motor over a specific amount of time. 
    
    @ATTENTION The program will not resume before this method is over.
    
    @param *servoId
    An array of the id's of servos that will be rotated. All the servos in this array will be rotated to the toState parameter in inMillis ms.
    
    @param toState [0 .. 100]
    The state you want the servos to be in at the end of the movement.
    
    @param inMillis [400 .. 60000]
    The amount of ms you want the rotation to take, this will influence the speed of the servos and thus make for a shorter or longer rotation.
    
    @param servos
    The amount of servos that you put in the *servoId array.
*/
void ServoController::MoveServosOverTime(int *servoId, int *toState, int inMillis, int servos)
{
    int millisPassed = 0;
    int lastMillis = millis();
     /* A fail safe so that the servo's don't break themselves */
    inMillis = max(inMillis, 400);
    
    /* The state of each servo at the beginning of the function */
    int fromState[servos];
    /* The delta movement of each servo */
    int deltaState[servos];
    /* For each servo set the fromState and deltaState variable in the appropriate arrays */
    for (int i = 0; i < servos; i++)
    {
        fromState[i] = servoState[servoId[i]];
        deltaState[i] = toState[i] - fromState[i];
    }
    
    /* Enter a while loop that executes for as long as you ordered the function to move the servos */
    while (millisPassed < inMillis)
    {
        /* Calculate the amount of millis that have passed since last loop */
        int deltaMillis = millis() - lastMillis;
        /* Set the new lastmillis to the current lastmillis */
        lastMillis = millis();
        /* Add the delta millis to the millispassed */
        millisPassed += deltaMillis;

        /* Calculate the percentage at which we are in the loop */        
        float percentage = ((float) millisPassed) / inMillis;
        /* For each servo in the servoId array move them the their new state */
        for (int i = 0; i < servos; i++)
        {
            /* Calculate their new state using the percentage, delta and fromState variables. */
            float newState = percentage * deltaState[i] + fromState[i];
            MoveServo(servoId[i], newState);
        }
        /* Delay for safety, too many actions could be really hard to run for the processor and thus it needs a period to rest. */
        delay(10);
    }
    
    /* Set the new position in the servoState Array */
    for (int i = 0; i < servos; i++)
    {
        servoState[servoId[i]] = toState[i];
    }
}

/* PRIVATE METHODS */

/** ResetServos
    Resets all the connected servos to state 0, this is necessary at the start of the program for example when we need to know the state of each servo.
*/
void ServoController::ResetServos(void)
{
    for (int s = 0; s < amountOfServos; s++)
        MoveServo(s, 0);
}

void ServoController::SoftReset(void)
{
    // MoveServoOverTime(baseRotServo, 0, 1000);
    int servos[7] = { 0, 1, 2, 3, 4, 5, 6 };
    int pos[7] = { 0, 0, 0, 0, 0, 0, 0 };
    MoveServosOverTime(servos, pos, 2000, 7);
    // MoveServoOverTime(armServo, 0, 1000);
    // MoveServoOverTime(armRotServo, 0, 1000);
    // MoveServoOverTime(handRotServo, 0, 1000);
    // MoveServoOverTime(handServo, 0, 1000);
}

/** StateToPulse
    Converts the given state value to a pulse value.
    
    @param servoId
    Each servo has its own minimum and maximum value, therefore we need to know which servo we are dealing with when we want to determine the pulse value.
    
    @param state
    The state value that you want converted to a pulse value.

    @return 
    Returns the pulse strenght that should be given to a servo to rotate to the given statae.
*/
int ServoController::StateToPulse(int servoId, int state)
{
    return map(state, 0, 100, _minServo[servoId], _maxServo[servoId]);
}

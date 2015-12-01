/**
    DataReader.cpp - ARM2D2 Library
    Created by Nick Belzer, December 1, 2015
*/

#include "Arduino.h"
#include "DataReader.h"

char inData[20];
char inChar = -1;
byte index = 0;

/** DataReader
	Constructs the datareader class, used to read commands from the serial console.
*/
DataReader::DataReader(void)
{
	Serial.begin(9600);
}

/** ReadCommand
	Read the command from the Serial line, if the command is equal to the given command it will return  true.
	
	@param command
	The command you want to check for true.
	
	@return 
	Returns true if the command was found, returns false if the command was not found.
*/
char DataReader::ReadCommand(char* command)
{	
	ReadSerial();
	
	if (strcmp(inData, command) == 0)
	{
		CleanStored();
		return (0);
	} else {
		return (1);
	}
}

char* DataReader::ReadSerial(void)
{
	/* While there is something to read for the program */
	while (Serial.available() > 0)
	{
		if (index < 19)
		{
			inChar = Serial.read();
			inData[index] = inChar;
			index++;
			inData[index] = '\0'; // Terminate the string
		}
	}
	
	return inData;
}

void DataReader::CleanStored(void)
{
	for (int i = 0; i < 19; i++)
	{
		inData[i] = 0;
	}
	index = 0;
}
/*
	DataReader.h - ARM2D2 Library
	Created by Nick Belzer, December 1, 2015
*/

#include "Arduino.h"

class DataReader
{
	public:
		DataReader(void);
		char ReadCommand(char* command);
		char* ReadSerial(void);
		void CleanStored(void);
	private:
	
};
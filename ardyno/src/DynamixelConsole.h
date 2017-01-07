#ifndef DYNAMIXEL_CONSOLE_H
#define DYNAMIXEL_CONSOLE_H

#include "DynamixelInterface.h"

class DynamixelConsole;

struct DynamixelCommand
{
	typedef DynamixelStatus (DynamixelConsole::*FunPtr)(int, char **);
	const char *mName;
	//const char *mDescription;
	FunPtr mCallback;
};

class Stream;
class DynamixelConsole
{
	public:
	
	DynamixelConsole(DynamixelInterface &aInterface, Stream &aConsole);
	
	void loop();
	
	private:
	
	void run();
	int parseCmd(char **argv);
	void printStatus(DynamixelStatus aStatus);
	void printData(const uint8_t *data, uint8_t length);
	
	DynamixelStatus ping(int argc, char **argv);
	DynamixelStatus read(int argc, char **argv);
	DynamixelStatus write(int argc, char **argv);
	DynamixelStatus reset(int argc, char **argv);
	DynamixelStatus reg_write(int argc, char **argv);
	DynamixelStatus action(int argc, char **argv);
	DynamixelStatus sync_write(int argc, char **argv);
	
	const static size_t sLineBufferSize=255;
	char mLineBuffer[sLineBufferSize+1];
	char *mLinePtr;
	DynamixelInterface &mInterface;
	Stream &mConsole;
	
	const static DynamixelCommand sCommand[];
};

#endif

/**
 * \file Dynamixel.cpp
*/

#include "Dynamixel.h"

uint8_t DynamixelPacket::checkSum()
{
	uint8_t result=mID+mLength+mInstruction;
	int n=0;
	if(mAddress!=255)
	{
		result+=mAddress;
		++n;
	}
	if(mDataLength!=255)
	{
		result+=mDataLength;
		++n;
	}
	for(int i=0; i<(mLength-2-n-mIDListSize); ++i)
	{
		result+=mData[i];
	}
	for(int i=0; i<mIDListSize; ++i)
	{
		result+=mIDList[i];
	}
	return ~result;
}



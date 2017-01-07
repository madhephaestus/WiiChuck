
#include "DynamixelInterface.h"

void DynamixelInterface::transaction(bool aExpectStatus)
{
	sendPacket(mPacket);
	if(aExpectStatus)
	{
		receivePacket(mPacket);
	}
	else
	{
		mPacket.mStatus=DYN_STATUS_OK;
	}
}

DynamixelStatus DynamixelInterface::read(uint8_t aID, uint8_t aAddress, uint8_t aSize, uint8_t *aPtr, uint8_t aStatusReturnLevel)
{
	mPacket=DynamixelPacket(aID, DYN_READ, 4, aPtr, aAddress, aSize);
	transaction(aStatusReturnLevel>0 && aID!=BROADCAST_ID);
	return mPacket.mStatus;
}

DynamixelStatus DynamixelInterface::write(uint8_t aID, uint8_t aAddress, uint8_t aSize, const uint8_t *aPtr, uint8_t aStatusReturnLevel)
{
	mPacket=DynamixelPacket(aID, DYN_WRITE, aSize+3, aPtr, aAddress);
	transaction(aStatusReturnLevel>1 && aID!=BROADCAST_ID);
	return mPacket.mStatus;
}

DynamixelStatus DynamixelInterface::regWrite(uint8_t aID, uint8_t aAddress, uint8_t aSize, const uint8_t *aPtr, uint8_t aStatusReturnLevel)
{
	mPacket=DynamixelPacket(aID, DYN_REG_WRITE, aSize+3, aPtr, aAddress);
	transaction(aStatusReturnLevel>1 && aID!=BROADCAST_ID);
	return mPacket.mStatus;
}

DynamixelStatus DynamixelInterface::syncWrite(uint8_t nID, const uint8_t *aID, uint8_t aAddress, uint8_t aSize, const uint8_t *aPtr, uint8_t aStatusReturnLevel)
{
	mPacket=DynamixelPacket(BROADCAST_ID, DYN_SYNC_WRITE, (aSize+1)*nID+4, aPtr, aAddress, aSize, nID, aID);
	transaction(false);
	return mPacket.mStatus;
}

DynamixelStatus DynamixelInterface::ping(uint8_t aID)
{
	mPacket=DynamixelPacket(aID, DYN_PING, 2, NULL);
	transaction(true);
	return mPacket.mStatus;
}

DynamixelStatus DynamixelInterface::action(uint8_t aID, uint8_t aStatusReturnLevel)
{
	mPacket=DynamixelPacket(aID, DYN_ACTION, 2, NULL);
	transaction(aStatusReturnLevel>1 && aID!=BROADCAST_ID);
	return mPacket.mStatus;
}

DynamixelStatus DynamixelInterface::reset(uint8_t aID, uint8_t aStatusReturnLevel)
{
	mPacket=DynamixelPacket(aID, DYN_RESET, 2, NULL);
	transaction(aStatusReturnLevel>1 && aID!=BROADCAST_ID);
	return mPacket.mStatus;
}



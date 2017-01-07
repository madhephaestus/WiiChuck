
#include "DynamixelInterface.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

#if __AVR__
// define TXEN, RXEN and RXCIE
#if !defined(TXEN)
#if defined(TXEN0)
#define TXEN TXEN0
#define RXEN RXEN0
#define RXCIE RXCIE0
#elif defined(TXEN1) // Some devices have uart1 but no uart0 (leonardo)
#define TXEN TXEN1
#define RXEN RXEN1
#define RXCIE RXCIE1
#endif
#endif
#endif //__AVR__

template<class T>
class DynamixelInterfaceImpl:public DynamixelInterface
{
	private:
	/** \brief Switch stream to read (receive)) mode */
	void readMode()
	{
		if(mDirectionPin!=NO_DIR_PORT)
		{
			digitalWrite(mDirectionPin, LOW);
		}
		else
		{
			setReadMode(mStream);
		}
	}
	
	/** \brief Switch stream to write (send) mode */
	void writeMode()
	{
		if(mDirectionPin!=NO_DIR_PORT)
		{
			digitalWrite(mDirectionPin, HIGH);
		}
		else
		{
			setWriteMode(mStream);
		}
	}
	
	public:
	
	/**
	 * \brief Constructor
	 * \param[in] aStreamController : stream controller that abstract real stream
	 * \param[in] aDirectionPin : direction pin, use NO_DIR_PORT if you do not one (default)
	 * \param[in] aTranferOwnership : if true, the stream will be deleted in the destructor
	*/
	DynamixelInterfaceImpl(T &aStream, uint8_t aDirectionPin=NO_DIR_PORT, bool aTranferOwnership=false):
		mStream(aStream), mDirectionPin(aDirectionPin), mStreamOwner(aTranferOwnership)
	{
		if(mDirectionPin!=NO_DIR_PORT)
		{
			digitalWrite(mDirectionPin, LOW);
			pinMode(mDirectionPin, OUTPUT);
		}
	}
	
	/**
	 * \brief Destructor
	 * Delete stream if it is owned by the instance
	 */
	~DynamixelInterfaceImpl()
	{
		if(mStreamOwner)
			delete &mStream;
	}
	
	/**
	 * \brief Start interface
	 * \param[in] aBaud : Baudrate
	 *
	 * Start the interface with desired baudrate, call once before using the interface
	*/
	void begin(unsigned long aBaud)
	{
		mStream.begin(aBaud);
		mStream.setTimeout(50); //warning : response delay seems much higher than expected for some operation (eg writing eeprom)
		readMode();
	}
	
	/**
	 * \brief Send a packet on bus
	 * \param[in] aPacket : Packet to send
	 *
	 * The function wait for the packet to be completly sent (using Stream.flush)
	*/
	void sendPacket(const DynamixelPacket &aPacket)
	{
		writeMode();
	
		mStream.write(0xFF);
		mStream.write(0xFF);
		mStream.write(aPacket.mID);
		mStream.write(aPacket.mLength);
		mStream.write(aPacket.mInstruction);
		uint8_t n=0;
		if(aPacket.mAddress!=255)
		{
			mStream.write(aPacket.mAddress);
			++n;
		}
		if(aPacket.mDataLength!=255)
		{
			mStream.write(aPacket.mDataLength);
			++n;
		}
		if(aPacket.mLength>(2+n))
		{
			if(aPacket.mIDListSize==0)
			{
				mStream.write(aPacket.mData, aPacket.mLength-2-n);
			}
			else
			{
				uint8_t *ptr=aPacket.mData;
				for(uint8_t i=0; i<aPacket.mIDListSize; ++i)
				{
					mStream.write(aPacket.mIDList[i]);
					mStream.write(ptr, aPacket.mDataLength);
					ptr+=aPacket.mDataLength;
				}
			}
		}
		mStream.write(aPacket.mCheckSum);
		mStream.flush();
		readMode();
	}
	/**
	 * \brief Receive a packet on bus
	 * \param[out] aPacket : Received packet. mData field must be previously allocated
	 *
	 * The function wait for a new packet on the bus. Timeout depends of timeout of the underlying stream.
	 * Return error code in case of communication error (timeout, checksum error, ...)
	*/
	void receivePacket(DynamixelPacket &aPacket)
	{
		static uint8_t buffer[3];
		aPacket.mIDListSize=0;
		aPacket.mAddress=255;
		aPacket.mDataLength=255;
		if(mStream.readBytes(buffer,2)<2)
		{
			aPacket.mStatus=DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
			return;
		}
		if(mStream.readBytes(buffer, 3)<3)
		{
			aPacket.mStatus=DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
			return;
		}
		aPacket.mID=buffer[0];
		aPacket.mLength=buffer[1];
		aPacket.mStatus=buffer[2];
		if(aPacket.mLength>2 && mStream.readBytes(reinterpret_cast<char*>(aPacket.mData), aPacket.mLength-2)<(aPacket.mLength-2))
		{
			aPacket.mStatus=DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
			return;
		}
		if(mStream.readBytes(reinterpret_cast<char*>(&(aPacket.mCheckSum)),1)<1)
		{
			aPacket.mStatus=DYN_STATUS_COM_ERROR | DYN_STATUS_TIMEOUT;
			return;
		}
		if(aPacket.checkSum()!=aPacket.mCheckSum)
		{
			aPacket.mStatus=DYN_STATUS_COM_ERROR | DYN_STATUS_CHECKSUM_ERROR;
		}
	}
	
	static const uint8_t NO_DIR_PORT=255;
	
	private:
	
	T &mStream;
	const uint8_t mDirectionPin;
	bool mStreamOwner;
};

template<class T>
void setReadMode(T &aStream);
template<class T>
void setWriteMode(T &aStream);

#if __AVR__
namespace {
//dirty trick to access protected member
class HardwareSerialAccess:public HardwareSerial
{
	public:
	volatile uint8_t * const ucsrb(){return _ucsrb;}
};
}

template<>
void setReadMode<HardwareSerial>(HardwareSerial &aStream)
{
	HardwareSerialAccess &stream=reinterpret_cast<HardwareSerialAccess&>(aStream);
	*(stream.ucsrb()) &= ~_BV(TXEN);
	*(stream.ucsrb()) |= _BV(RXEN);
	*(stream.ucsrb()) |= _BV(RXCIE);
}

template<>
void setWriteMode<HardwareSerial>(HardwareSerial &aStream)
{
	HardwareSerialAccess &stream=reinterpret_cast<HardwareSerialAccess&>(aStream);
	*(stream.ucsrb()) &= ~_BV(RXEN);
	*(stream.ucsrb()) |= _BV(RXCIE);
	*(stream.ucsrb()) |= _BV(TXEN);
}

#elif __arc__

//Arduino 101 specific code

template<>
void setReadMode<HardwareSerial>(HardwareSerial &aStream)
{
	//enable pull up to avoid noise on the line
	pinMode(1, INPUT);
	digitalWrite(1, HIGH);
	// disconnect UART TX and connect UART RX
	SET_PIN_MODE(16, GPIO_MUX_MODE);
	SET_PIN_MODE(17, UART_MUX_MODE);
}

template<>
void setWriteMode<HardwareSerial>(HardwareSerial &aStream)
{
	// disconnect UART RX and connect UART TX
	SET_PIN_MODE(17, GPIO_MUX_MODE);
	SET_PIN_MODE(16, UART_MUX_MODE);
}

#endif

namespace{
class DynSoftwareSerial:public SoftwareSerial
{
	public:
	DynSoftwareSerial(uint8_t aRxPin, uint8_t aTxPin):
		SoftwareSerial(aRxPin, aTxPin), mTxPin(aTxPin)
	{}
	
	void enableTx(){pinMode(mTxPin, OUTPUT);}
	void disableTx(){pinMode(mTxPin, INPUT);}
	
	private:
	uint8_t mTxPin;
};
}

template<>
void setReadMode<DynSoftwareSerial>(DynSoftwareSerial &aStream)
{
	aStream.disableTx();
	aStream.listen();
}

template<>
void setWriteMode<DynSoftwareSerial>(DynSoftwareSerial &aStream)
{
	aStream.stopListening();
	aStream.enableTx();
}



DynamixelInterface *createSerialInterface(HardwareSerial &aSerial)
{
	return new DynamixelInterfaceImpl<HardwareSerial>(aSerial);
}

DynamixelInterface *createSerialInterface(HardwareSerial &aSerial, uint8_t aDirectionPin)
{
	return new DynamixelInterfaceImpl<HardwareSerial>(aSerial, aDirectionPin);
}

DynamixelInterface *createSoftSerialInterface(uint8_t aRxPin, uint8_t aTxPin)
{
	DynSoftwareSerial &serial=*new DynSoftwareSerial(aRxPin, aTxPin);
	return new DynamixelInterfaceImpl<DynSoftwareSerial>(serial, DynamixelInterfaceImpl<DynSoftwareSerial>::NO_DIR_PORT, true);
}

DynamixelInterface *createSoftSerialInterface(uint8_t aRxPin, uint8_t aTxPin, uint8_t aDirectionPin)
{
	DynSoftwareSerial &serial=*new DynSoftwareSerial(aRxPin, aTxPin);
	return new DynamixelInterfaceImpl<DynSoftwareSerial>(serial, aDirectionPin, true);
}






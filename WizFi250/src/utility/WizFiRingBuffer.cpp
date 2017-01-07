/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */
#include "WizFiRingBuffer.h"

WizFiRingBuffer::WizFiRingBuffer( int size )
{
	bufferSize = size+1;
	bufferTail = 0;
	bufferHead = 0;
	buffer_p = ( byte* )malloc( bufferSize );
        memset( buffer_p, 0, bufferSize );
}

WizFiRingBuffer::~WizFiRingBuffer()
{
	if( buffer_p )
          free( buffer_p );
}

// public functions
int WizFiRingBuffer::reset()
{
	bufferHead = 0;
	bufferTail = 0;

	memset(buffer_p, 0, bufferSize);
}

int WizFiRingBuffer::available(void)
{
  int ByteCount = (bufferSize + bufferHead - bufferTail) % bufferSize;
  return ByteCount;
}

bool WizFiRingBuffer::isFull()
{
	if( available() >= bufferSize)	return true;

	return false;
}

int WizFiRingBuffer::getBufferSize()
{
	return bufferSize;
}

int WizFiRingBuffer::read(void)
{
  if (bufferHead == bufferTail) {
    return -1;
  }
  else {
    byte c = buffer_p[bufferTail];
    bufferTail = (bufferTail + 1) % bufferSize;
    if(bufferHead == bufferTail) {
      bufferTail = 0;
      bufferHead = 0;
    }
    return c;
  }
}

byte WizFiRingBuffer::write( int c )
{
  //if ((bufferHead + 1) % bufferSize == bufferTail) {
  //  return -1;
  //}
  buffer_p[bufferHead] = c;
  bufferHead = (bufferHead + 1) % bufferSize;
  return 0;
}

void WizFiRingBuffer::remove(int n)
{
  if(bufferHead != bufferTail) {
    bufferTail = (bufferTail + n) % bufferSize;
  }
}

int WizFiRingBuffer::peek(void)
{
  if (bufferHead == bufferTail) {
    return -1;
  }
  else {
    return buffer_p[bufferTail];
  }
}

int WizFiRingBuffer::peek(int n) {

  if (bufferHead == bufferTail) {
    return -1;
  }
  else {
    return buffer_p[(bufferTail + n) % bufferSize];
  }
}

void WizFiRingBuffer::init()
{
	reset();
}

void WizFiRingBuffer::push(char c)
{
	write(c);
}

bool WizFiRingBuffer::endsWith(const char* str)
{
	int buffer_len;

	int findStrLen = strlen(str);

	char *p1 = (char*)&str[0];
	char *p2 = p1 + findStrLen;

	unsigned int tail = bufferHead - findStrLen;

	for(char *p=p1; p<p2; p++)
	{
		if(*p != buffer_p[tail])
		{
			return false;
		}

		tail++;

		if( tail == bufferSize)		tail = 0;
	}

	return true;
}

char* WizFiRingBuffer::FindStr(const char* str)
{
	char *p_find_start = (char*)&str[0];
	char *p_src_start  = (char*)&buffer_p[bufferTail];

	return strstr((char*)p_src_start,(char*)p_find_start);
}


int WizFiRingBuffer::getString(char* dest, int size)
{
	char *p = (char*)&buffer_p[bufferTail];
	strncpy((char*)dest,(char*)p,size);
	return 0;
}


int WizFiRingBuffer::getLine(char* dest, int size, char sep, int skipchar)
{
	int i,len=0,byteCnt=0;
	char ch;


	byteCnt = available();

	for(i=0; i<byteCnt; i++)
	{
		ch = (char)read();
		if(ch == sep)		break;

		if(i > size)
		{
			continue;
		}

		dest[i] = ch;
	}
	len = i;


	for(i=0;i<skipchar;i++)	read();

	return len;
}

void WizFiRingBuffer::printbuffer()
{
	char *p = (char*)&buffer_p[bufferTail];
#if defined(ARDUINO_ARCH_SAMD) 
	SerialUSB.println(p);
#else
	Serial.println(p);
#endif
}

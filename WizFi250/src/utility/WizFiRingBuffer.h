/*
 * Copyright (C) WIZnet, Inc. All rights reserved.
 * Use is subject to license terms.
 */
#ifndef WIZFI250_LIGHT_UTILITY_RINGBUFFER_H_
#define WIZFI250_LIGHT_UTILITY_RINGBUFFER_H_


#include "Arduino.h"

class WizFiRingBuffer
{
private:

	int bufferSize;
	unsigned int bufferHead, bufferTail;
	byte *buffer_p;

public:
	WizFiRingBuffer(int size);
        ~WizFiRingBuffer();
    int reset();
	int available(void);
	bool isFull();
	int getBufferSize();
	int peek(void);
	int peek(int);
	void remove(int);
	int read(void);
	byte write(int);

	void	init();
	void	push(char c);

	bool	endsWith	(const char* str);
	char*	FindStr		(const char* str);
	int		getString	(char* dest, int size);
	int		getLine		(char* dest, int size, char sep, int skipchar);

	void	printbuffer	();
};

#endif /* WIZFI250_LIGHT_UTILITY_RINGBUFFER_H_ */

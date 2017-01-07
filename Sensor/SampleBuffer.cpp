#include "SampleBuffer.h"
#include <math.h>
#include <limits.h>

FloatSampleBuffer::FloatSampleBuffer()
{
	clear();
}

void FloatSampleBuffer::addSample(float sample)
{
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		if (isnan(samples[i]))
		{
			samples[i] = sample;
			samples[(i + 1) % NUM_SAMPLES] = NAN;
			break;
		}
	}
}

float FloatSampleBuffer::getAverage()
{
	float accumulator = 0.f;
	int numSamples = 0;
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		if (!isnan(samples[i]))
		{
			accumulator += samples[i];
			numSamples++;
		}
	}
	return accumulator / numSamples;
}

void FloatSampleBuffer::clear()
{
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		samples[i] = NAN;
	}
}

IntSampleBuffer::IntSampleBuffer()
{
	clear();
}

void IntSampleBuffer::addSample(int sample)
{
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		if (INT_MIN == samples[i])
		{
			samples[i] = sample;
			samples[(i + 1) % NUM_SAMPLES] = INT_MIN;
			break;
		}
	}
}

int IntSampleBuffer::getAverage()
{
	int accumulator = 0.f;
	int numSamples = 0;
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		if (INT_MIN != samples[i])
		{
			accumulator += samples[i];
			numSamples++;
		}
	}
	return accumulator / numSamples;
}

void IntSampleBuffer::clear()
{
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		samples[i] = INT_MIN;
	}
}
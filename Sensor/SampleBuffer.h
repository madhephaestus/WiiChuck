#ifndef __SAMPLEBUFFER_H__
#define __SAMPLEBUFFER_H__

class FloatSampleBuffer
{
public:
	FloatSampleBuffer();
	void addSample(float sample);
	float getAverage();
	void clear();

private:
	static const int NUM_SAMPLES = 11;
	float samples[11];
};

class IntSampleBuffer
{
public:
	IntSampleBuffer();
	void addSample(int sample);
	int getAverage();
	void clear();

private:
	static const int NUM_SAMPLES = 11;
	int samples[11];
};

#endif
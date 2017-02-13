#ifndef Nunchuck_h
#define Nunchuck_h
#include "controllers/Accessory.h"

class Nunchuck : public Accessory
{
public:
	Nunchuck(uint8_t data_pin, uint8_t sclk_pin);
    void printInputs(Stream& stream);

	int getJoyX();
	int getJoyY();

	int getRollAngle();
	int getPitchAngle();
	int getAccelX();
	int getAccelY();
	int getAccelZ();

	boolean checkButtonC();
	boolean checkButtonZ();
	int getAnalog(FunctionMapName name);
	boolean getDigital(ButtonMapName name);
private:
	int _rawgetJoyX();
	int _rawgetJoyY();
	int32_t _joy_x_center, _joy_y_center;

};

#endif

#ifndef Nunchuck_h
#define Nunchuck_h
#include "Accessory.h"


// see http://wiibrew.org/wiki/Wiimote/Extension_Nunchuck
#define joyXBytes     UNUSED, 0, 0, UNUSED ,0   , 0   , BYTE0, BIT0, BIT7
#define joyYBytes     UNUSED, 0, 0, UNUSED ,0   , 0   , BYTE1, BIT0, BIT7
#define accelXBytes   UNUSED, 0, 0, BYTE2  ,BIT0, BIT7, BYTE5, BIT2, BIT3
#define accelYBytes   UNUSED, 0, 0, BYTE3  ,BIT0, BIT7, BYTE5, BIT4, BIT5
#define accelZBytes   UNUSED, 0, 0, BYTE4  ,BIT0, BIT7, BYTE5, BIT6, BIT7

#define buttonCBits   BYTE5,BIT1,true
#define buttonZBits   BYTE5,BIT0,true


class Nunchuck : public Accessory
{
public:
    Nunchuck();
    void printInputs(Stream& stream = Serial);

    int getJoyX();
    int getJoyY();

    float getRollAngle();
    float getPitchAngle();
    int getAccelX();
    int getAccelY();
    int getAccelZ();


    boolean checkButtonC();
    boolean checkButtonZ();


    class joyX : public Accessory::Mapping
    {
    public:
        joyX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        joyX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};

        unsigned int  mapVar();
        void printMap(Stream& stream = Serial);
        const uint16_t myMin = 0;
        const uint16_t myZero = 125;
        const uint16_t myMax = 255;

    };

    class joyY : public Accessory::Mapping
    {
    public:
        joyY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        joyY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};

        unsigned int  mapVar();
        void printMap(Stream& stream = Serial);
        const uint16_t myMin = 0;
        const uint16_t myZero = 125;
        const uint16_t myMax = 255;
    };

    class roll : public Accessory::Mapping
    {
    public:
        roll(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        roll(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};

        unsigned int  mapVar();
        void printMap(Stream& stream = Serial);
        const int16_t myMin = -180;
        const int16_t myZero = 0;
        const int16_t myMax = 180;
    };


    class pitch : public Accessory::Mapping
    {
    public:
        pitch(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        pitch(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};

        unsigned int  mapVar();
        void printMap(Stream& stream = Serial);
        const int16_t myMin = -180;
        const int16_t myZero = 0;
        const int16_t myMax = 180;
    };


    class buttonC : public Accessory::Mapping
    {
    public:
        buttonC(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        buttonC(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};

        void printMap(Stream& stream = Serial);
        unsigned int  mapVar();
    };

    class buttonZ : public Accessory::Mapping
    {
    public:
        buttonZ(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        buttonZ(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown) : Mapping( chan, max, zero, min, cooldown) {};

        void printMap(Stream& stream = Serial);
        unsigned int  mapVar();
    };

};

#endif

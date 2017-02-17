#ifndef Classic_h
#define Classic_h
#include "controllers/Accessory.h"
#define joyXLeftBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE0,  BIT0, BIT5
#define joyXRightBytes	BYTE0,   BIT6,  BIT7,    BYTE1,     BIT6,  BIT7, BYTE2,  BIT7, BIT7
#define joyYLeftBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE1,  BIT0, BIT5
#define joyYRightBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE2,  BIT0, BIT4
#define triggerLeftBytes	UNUSED,     0,     0,    BYTE2,     BIT5,  BIT6, BYTE3,  BIT5, BIT7
#define triggerRightBytes	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE3,  BIT0, BIT4
#define padRightBytes	BYTE5,BIT1,true
#define padDownBytes	BYTE5,BIT1,true
#define padUpBytes	BYTE5,BIT1,true
#define padLeftBytes	BYTE5,BIT1,true
#define buttonXBytes	BYTE5,BIT1,true
#define buttonYBytes	BYTE5,BIT1,true
#define buttonABytes	BYTE5,BIT1,true
#define buttonBBytes	BYTE5,BIT1,true
#define buttonMinusBytes	BYTE5,BIT1,true
#define buttonHomeBytes	BYTE5,BIT1,true
#define buttonPlusBytes	BYTE5,BIT1,true


class Classic : public Accessory
{
    Classic(uint8_t data_pin, uint8_t sclk_pin);
    void printInputs(Stream& stream);

    int getJoyXLeft();
    int getJoyXRight();
    int getJoyYLeft();
    int getJoyYRight();
    int getTriggerLeft();
    int getTriggerRight();
    int getPadRight();
    int getPadDown();
    int getPadUp();
    int getPadLeft();
    int getButtonX();
    int getButtonY();
    int getButtonA();
    int getButtonB();
    int getButtonMinus();
    int getButtonHome();
    int getButtonPlus();




    class joyXLeft : public Accessory::Mapping
    {
    public:
        joyXLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);
        const uint16_t myMin = 0;
        const uint16_t myZero = 125;
        const uint16_t myMax = 255;

    };


    class joyXRight : public Accessory::Mapping
    {
    public:
        joyXRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);
        const uint16_t myMin = 0;
        const uint16_t myZero = 125;
        const uint16_t myMax = 255;

    };


    class joyYLeft : public Accessory::Mapping
    {
    public:
        joyYLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);
        const uint16_t myMin = 0;
        const uint16_t myZero = 125;
        const uint16_t myMax = 255;

    };


    class joyYRight : public Accessory::Mapping
    {
    public:
        joyYRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);
        const uint16_t myMin = 0;
        const uint16_t myZero = 125;
        const uint16_t myMax = 255;

    };


    class triggerLeft : public Accessory::Mapping
    {
    public:
        triggerLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);
        const uint16_t myMin = 0;
        const uint16_t myZero = 125;
        const uint16_t myMax = 255;

    };


    class triggerRight : public Accessory::Mapping
    {
    public:
        triggerRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);
        const uint16_t myMin = 0;
        const uint16_t myZero = 125;
        const uint16_t myMax = 255;

    };


    class padRight : public Accessory::Mapping
    {
    public:
        padRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };


    class padDown : public Accessory::Mapping
    {
    public:
        padDown(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };


    class padUp : public Accessory::Mapping
    {
    public:
        padUp(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };


    class padLeft : public Accessory::Mapping
    {
    public:
        padLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };


    class buttonX : public Accessory::Mapping
    {
    public:
        buttonX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };


    class buttonY : public Accessory::Mapping
    {
    public:
        buttonY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };


    class buttonA : public Accessory::Mapping
    {
    public:
        buttonA(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };


    class buttonB : public Accessory::Mapping
    {
    public:
        buttonB(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };


    class buttonMinus : public Accessory::Mapping
    {
    public:
        buttonMinus(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };


    class buttonHome : public Accessory::Mapping
    {
    public:
        buttonHome(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };


    class buttonPlus : public Accessory::Mapping
    {
    public:
        buttonPlus(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };



};
#endif



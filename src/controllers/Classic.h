#define joyXLeftBits	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE0,  BIT0, BIT5
#define joyXRightBits	BYTE0,   BIT6,  BIT7,    BYTE1,     BIT6,  BIT7, BYTE2,  BIT7, BIT7
#define joyYLeftBits	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE1,  BIT0, BIT5
#define joyYRightBits	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE2,  BIT0, BIT4
#define triggerLeftBits	UNUSED,     0,     0,    BYTE2,     BIT5,  BIT6, BYTE3,  BIT5, BIT7
#define triggerRightBits	UNUSED,     0,     0,   UNUSED,        0,     0, BYTE3,  BIT0, BIT4
#define padRightBits
#define padDownBits
#define padUpBits
#define padLeftBits
#define buttonXBits
#define buttonYBits
#define buttonABits
#define buttonBBits
#define buttonMinusBits
#define buttonHomeBits
#define buttonPlusBits


class Classic : public Accessory
{
    Classic(uint8_t data_pin, uint8_t sclk_pin);
    void printInputs(Stream& stream);

    int joyXLeft();
    int joyXRight();
    int joyYLeft();
    int joyYRight();
    int triggerLeft();
    int triggerRight();
    int padRight();
    int padDown();
    int padUp();
    int padLeft();
    int buttonX();
    int buttonY();
    int buttonA();
    int buttonB();
    int buttonMinus();
    int buttonHome();
    int buttonPlus();



    int getJoyXLeft();

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

    int getJoyXRight();

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

    int getJoyYLeft();

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

    int getJoyYRight();

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

    int getTriggerLeft();

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

    int getTriggerRight();

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

    int getPadRight();

    class padRight : public Accessory::Mapping
    {
    public:
        padRight(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };

    int getPadDown();

    class padDown : public Accessory::Mapping
    {
    public:
        padDown(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };

    int getPadUp();

    class padUp : public Accessory::Mapping
    {
    public:
        padUp(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };

    int getPadLeft();

    class padLeft : public Accessory::Mapping
    {
    public:
        padLeft(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };

    int getButtonX();

    class buttonX : public Accessory::Mapping
    {
    public:
        buttonX(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };

    int getButtonY();

    class buttonY : public Accessory::Mapping
    {
    public:
        buttonY(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };

    int getButtonA();

    class buttonA : public Accessory::Mapping
    {
    public:
        buttonA(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };

    int getButtonB();

    class buttonB : public Accessory::Mapping
    {
    public:
        buttonB(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };

    int getButtonMinus();

    class buttonMinus : public Accessory::Mapping
    {
    public:
        buttonMinus(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };

    int getButtonHome();

    class buttonHome : public Accessory::Mapping
    {
    public:
        buttonHome(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };

    int getButtonPlus();

    class buttonPlus : public Accessory::Mapping
    {
    public:
        buttonPlus(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) : Mapping( chan, max, zero, min) {};
        uint16_t mapVar();
        void printMap(Stream& stream);

    };



}


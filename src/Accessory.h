#ifndef Accessory_h
#define Accessory_h

#include "Arduino.h"
#include <Servo.h>

#define I2C_ADDR		0x52

#define BYTE0 0
#define BYTE1 1
#define BYTE2 2
#define BYTE3 3
#define BYTE4 4
#define BYTE5 5
#define UNUSED 255

#define BIT0 0
#define BIT1 1
#define BIT2 2
#define BIT3 3
#define BIT4 4
#define BIT5 5
#define BIT6 6
#define BIT7 7

typedef enum _controllertype {
    Unknown,
    NUNCHUCK,
    WIICLASSIC,
    GuitarHeroController,
    GuitarHeroWorldTourDrums,
    DrumController,
    DrawsomeTablet,
    Turntable
} ControllerType;

class Accessory {
public:
    Accessory();

    uint8_t* getDataArray();
    void setDataArray(uint8_t data[6]);

    void printInputs(Stream& stream = Serial);

    void begin();
    boolean readData();

    void enableEncryption(bool enc);

    void addMultiplexer(uint8_t iic, uint8_t sw);
    void switchMultiplexer();
    static void switchMultiplexer(uint8_t iic, uint8_t sw);

    int decodeInt(uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
                  uint8_t csbbyte, uint8_t csbstart, uint8_t csbend,
                  uint8_t lsbbyte,uint8_t lsbstart, uint8_t lsbend);
                  
    int decodeInt(uint8_t mmsbbyte, uint8_t mmsbstart, uint8_t mmsbend,
                  uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
                  uint8_t csbbyte, uint8_t csbstart, uint8_t csbend,
                  uint8_t lsbbyte,uint8_t lsbstart, uint8_t lsbend);

    bool decodeBit(uint8_t byte, uint8_t bit, bool activeLow);

    void printMaps(Stream& stream = Serial);
    uint8_t getMapCount();
    void removeMaps();
    void removeMap(uint8_t id);
    ControllerType getControllerType();

    static int smap(int16_t val,int16_t aMax, int16_t aMid, int16_t aMin, int16_t sMax, int16_t sZero,int16_t sMin);


    class Mapping {
    public:
        Mapping();
        Mapping(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min);
        Mapping(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min, uint16_t cooldown);
        virtual unsigned int  mapVar();
        virtual void printMap(Stream& stream = Serial);
        // Data Parsing

        Mapping* next;
        Accessory* controller;

        void addServo(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min);
        void update();
        
        ControllerType getControllerType();
    protected:
        bool _encrypted=false;
        Servo servo;
        uint8_t channel;
        uint32_t _cooldown=0;
        uint32_t _cooldownCount=0;

        uint8_t servoMax;
        uint8_t servoZero;
        uint8_t servoMin;
    };
    uint8_t addMap(Mapping* m);

protected:
    bool _encrypted;
    ControllerType type;
    // allow sub classes to view the data

    uint8_t _dataarray[6];
    uint8_t _multiplexI2C = 0;
    uint8_t _multiplexSwitch;

    //uint8_t _key_table_1[16]= {0xe0,0x7d,0xe0,0x7d,0xe0,0x7d,0xe0,0x7d,0xe0,0x7d,0x38,0x54,0xbb,0x79,0x01,0x43};
    uint8_t _key_table_1[16]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint8_t decryptByte(uint8_t byte,uint8_t address);
    virtual void initBytes();

    ControllerType identifyController();
    Mapping* firstMap;
    boolean _burstRead(uint8_t addr = 0);
    void _writeRegister(uint8_t reg, uint8_t value);
    void _burstWriteWithAddress(uint8_t addr,uint8_t* arr,uint8_t size);

private:
    static void sendMultiSwitch(uint8_t iic, uint8_t sw);

    uint8_t mapCount;
    void _applyMaps();
};

#endif
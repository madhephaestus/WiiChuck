#ifndef Accessory_h
#define Accessory_h

#include "Arduino.h"

class Accessory {
  public:
    Accessory(uint8_t bytes[6]);
    Accessory();
    
    uint8_t* getDataArray();
    void setDataArray(uint8_t data[6]);
  
    void printInputs(Stream& stream);
    
    int   decodeInt(uint8_t msbbyte,uint8_t msbstart,uint8_t msbend,
                           uint8_t csbbyte, uint8_t csbstart, uint8_t csbend,
                           uint8_t lsbbyte, uint8_t lsbstart, uint8_t lsbend);
    bool  decodeBit(uint8_t byte, uint8_t bit, bool activeLow); 
  
  private:
    uint8_t _dataarray[8];
    



};

typedef enum _inType {ANALOG,DIGITAL} inType;

typedef struct inputMap {
  inType  type;
  
  // digital in params
  uint8_t dByte;
  uint8_t dBit;
  bool    dActiveLow;
  
  // analog in params
  uint8_t msbbyte;
  uint8_t msbstart;
  uint8_t msbend;
  
  uint8_t msbbyte;
  uint8_t msbstart;
  uint8_t msbend;
  
  uint8_t csbbyte;
  uint8_t csbstart;
  uint8_t csbend;
  
  uint8_t lsbbyte;
  uint8_t lsbstart;
  uint8_t lsbend;
  
  uint8_t anmax;
  uint8_t anzero;
  uint8_t anmin;
  
  uint8_t sevomax;
  uint8_t servozero;
  uint8_t servomin;
  
  Servo   servo;
}

#endif

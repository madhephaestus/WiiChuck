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

#endif

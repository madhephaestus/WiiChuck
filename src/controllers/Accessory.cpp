#include "Accessory.h"


Accessory::Accessory(uint8_t bytes[6]){
  setDataArray(bytes);
}

Accessory::Accessory(){
  uint8_t a[] = {0,0,0,0,0,0};
  setDataArray(a);
}

uint8_t* Accessory::getDataArray(){
  return _dataarray;
}

void Accessory::setDataArray(uint8_t data[6]){
  for(int i=0; i<6; i++) _dataarray[i] = data[i];
}
  
void Accessory::printInputs(Stream& stream){
  stream.print("Accessory Bytes:\t");
  for(int i=0; i<6; i++) {
         if (_dataarray[i]<0x10) {stream.print("0");} 
         stream.print(_dataarray[i],HEX); 
         stream.print(" "); 
  }
  stream.println("");
}

int   Accessory::decodeInt(uint8_t msbbyte,uint8_t msbstart,uint8_t msbend,
                           uint8_t csbbyte, uint8_t csbstart, uint8_t csbend,
                           uint8_t lsbbyte, uint8_t lsbstart, uint8_t lsbend){
// 5 bit int split across 3 bytes. what... the... fuck... nintendo...

  if (msbbyte>5) return false;
  if (csbbyte>5) return false;
  if (lsbbyte>5) return false;

  uint32_t analog=0;
  uint32_t lpart;
  lpart = _dataarray[lsbbyte];
  lpart = lpart >> lsbstart;
  lpart = lpart &  (0xFF>>(7-(lsbend-lsbstart)) ) ;
  
  uint32_t cpart;
  cpart = _dataarray[csbbyte];
  cpart = cpart >> csbstart;
  cpart = cpart &  (0xFF>>(7-(csbend-csbstart)) ) ;
  
  cpart = cpart << ((lsbend-lsbstart)+1);
  
  uint32_t mpart;
  mpart = _dataarray[msbbyte];
  mpart = mpart >> msbstart;
  mpart = mpart &  (0xFF>>(7-(msbend-msbstart)) ) ;
  
  mpart = mpart << ( ((lsbend-lsbstart)+1) + ((csbend-csbstart)+1) ) ;
  
  analog = lpart | cpart | mpart;
  
  return analog;
  
}

bool  Accessory::decodeBit(uint8_t byte, uint8_t bit, bool activeLow){
  if (byte>5) return false;
  uint8_t swb = _dataarray[byte];
  uint8_t sw   = (swb>>bit) & 0x01;
  return activeLow?(!sw):(sw);
} 

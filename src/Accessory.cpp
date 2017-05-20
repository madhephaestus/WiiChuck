#include "Accessory.h"
#include <Wire.h>

#include "sWire.h"
#include "Servo.h"

//#define WiiChickUseSWiic

//#ifdef WiiChickUseSWiic
//  sWire myWire;
//#else
#define myWire Wire
//#endif


Accessory::Accessory(uint8_t data_pin, uint8_t sclk_pin) {
//#ifdef WiiChickUseSWiic
//  myWire.setiicpins(data_pin,sclk_pin);
//#endif

}
/**
 * Reads the device type from the controller
 */
ControllerType Accessory::getControllerType(){
  return type;
}  
 
ControllerType Accessory::identifyController() {
    //Serial.println("Reading periph bytes");
    _burstReadWithAddress(0xfa);
    //printInputs(Serial);

    if (_dataarray[4] == 0x01)
        if (_dataarray[5] == 0x01)
            return WIICLASSIC; // Classic Controller

    if (_dataarray[4] == 0x00)
        if (_dataarray[5] == 0x00)
            return NUNCHUCK; // nunchuck


    if (_dataarray[0] == 0x00)
        if (_dataarray[1] == 0x00)
            if (_dataarray[2] == 0xa4)
                if (_dataarray[3] == 0x20)
                    if (_dataarray[4] == 0x01)
                        if (_dataarray[5] == 0x03)
                            return GuitarHeroController; // Guitar Hero Controller

    if (_dataarray[0] == 0x01)
        if (_dataarray[1] == 0x00)
            if (_dataarray[2] == 0xa4)
                if (_dataarray[3] == 0x20)
                    if (_dataarray[4] == 0x01)
                        if (_dataarray[5] == 0x03)
                            return GuitarHeroWorldTourDrums; // Guitar Hero World Tour Drums

    if (_dataarray[0] == 0x03)
        if (_dataarray[1] == 0x00)
            if (_dataarray[2] == 0xa4)
                if (_dataarray[3] == 0x20)
                    if (_dataarray[4] == 0x01)
                        if (_dataarray[5] == 0x03)
                            return Turntable; // Guitar Hero World Tour Drums

    if (_dataarray[0] == 0x00)
        if (_dataarray[1] == 0x00)
            if (_dataarray[2] == 0xa4)
                if (_dataarray[3] == 0x20)
                    if (_dataarray[4] == 0x01)
                        if (_dataarray[5] == 0x11)
                            return DrumController; // Taiko no Tatsujin TaTaCon (Drum controller)

    if (_dataarray[0] == 0xFF)
        if (_dataarray[1] == 0x00)
            if (_dataarray[2] == 0xa4)
                if (_dataarray[3] == 0x20)
                    if (_dataarray[4] == 0x00)
                        if (_dataarray[5] == 0x13)
                            return DrawsomeTablet; // Drawsome Tablet


    return Unknown;
}

/*
 * public function to read data
 */
void Accessory::readData() {

    _burstRead();
    _applyMaps();
}

uint8_t* Accessory::getDataArray() {
    return _dataarray;
}

void Accessory::initBytes() {
        //Serial.println("Init Periph..");
        _writeRegister(0xF0, 0x55);
        _writeRegister(0xFB, 0x00);
        delay(100);
        
        type = identifyController();
        delay(100);
        
    if (_encrypted) {
        //Serial.println("Beginning Encrypted Comms");

        delay(100);
        
        _writeRegister(0xF0, 0xAA); // enable enc mode?
        delay(90);

        Accessory::_burstWriteWithAddress(0x40,_key_table_1,8);
        Accessory::_burstWriteWithAddress(0x48,_key_table_1+0x8,8);
        delay(100);


        //_writeRegister(0x40, 0x00);

    }
}



void Accessory::setDataArray(uint8_t data[6]) {
    for (int i = 0; i < 6; i++)
        _dataarray[i] = data[i];
}

void Accessory::printInputs(Stream& stream) {
    stream.print("Accessory Bytes:\t");
    for (int i = 0; i < 6; i++) {
        if (_dataarray[i] < 0x10) {
            stream.print("0");
        }
        stream.print(_dataarray[i], HEX);
        stream.print(" ");
    }
    stream.println("");
}

int Accessory::decodeInt(uint8_t mmsbbyte, uint8_t mmsbstart, uint8_t mmsbend,
                         uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
                         uint8_t csbbyte, uint8_t csbstart, uint8_t csbend,
                         uint8_t lsbbyte,uint8_t lsbstart, uint8_t lsbend) {
// 6 bit int split across 3 bytes in 4 parts.... :(
    bool msbflag=false,csbflag=false,lsbflag=false,mmsbflag=false;
    if (msbbyte > 5)
        msbflag=true;
    if (csbbyte > 5)
        csbflag=true;
    if (lsbbyte > 5)
        lsbflag=true;
    if (mmsbbyte > 5)
        mmsbflag=true;
    uint32_t analog = 0;
    uint32_t lpart=0;
    lpart = (lsbflag)?0:_dataarray[lsbbyte];
    lpart = lpart >> lsbstart;
    lpart = lpart & (0xFF >> (7 - (lsbend - lsbstart)));

    uint32_t cpart=0;
    cpart = (csbflag)?0:_dataarray[csbbyte];
    cpart = cpart >> csbstart;
    cpart = cpart & (0xFF >> (7 - (csbend - csbstart)));

    cpart = cpart << ((lsbend - lsbstart) + 1);

    uint32_t mpart=0;
    mpart = (msbflag)?0:_dataarray[msbbyte];
    mpart = mpart >> msbstart;
    mpart = mpart & (0xFF >> (7 - (msbend - msbstart)));

    mpart = mpart << (((lsbend - lsbstart) + 1) + ((csbend - csbstart) + 1));
    
    uint32_t mmpart=0;
    mmpart = (mmsbflag)?0:_dataarray[mmsbbyte];
    mmpart = mmpart >> mmsbstart;
    mmpart = mmpart & (0xFF >> (7 - (mmsbend - mmsbstart)));

    mmpart = mmpart << ( ((lsbend - lsbstart) + 1) + ((csbend - csbstart) + 1) + ((msbend - msbstart) + 1));

    analog = lpart | cpart | mpart | mmpart;
    //analog = analog + offset;
    //analog = (analog*scale);

    return analog;                         
                         }

int Accessory::decodeInt(uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
                         uint8_t csbbyte, uint8_t csbstart, uint8_t csbend,
                         uint8_t lsbbyte,uint8_t lsbstart, uint8_t lsbend) {
// 5 bit int split across 3 bytes. what... the... fuck... nintendo...
    bool msbflag=false,csbflag=false,lsbflag=false;
    if (msbbyte > 5)
        msbflag=true;
    if (csbbyte > 5)
        csbflag=true;
    if (lsbbyte > 5)
        lsbflag=true;

    uint32_t analog = 0;
    uint16_t lpart=0;
    lpart = (lsbflag)?0:_dataarray[lsbbyte];
    lpart = lpart >> lsbstart;
    lpart = lpart & (0xFF >> (7 - (lsbend - lsbstart)));

    uint16_t cpart=0;
    cpart = (csbflag)?0:_dataarray[csbbyte];
    cpart = cpart >> csbstart;
    cpart = cpart & (0xFF >> (7 - (csbend - csbstart)));

    cpart = cpart << ((lsbend - lsbstart) + 1);

    uint16_t mpart=0;
    mpart = (msbflag)?0:_dataarray[msbbyte];
    mpart = mpart >> msbstart;
    mpart = mpart & (0xFF >> (7 - (msbend - msbstart)));

    mpart = mpart << (((lsbend - lsbstart) + 1) + ((csbend - csbstart) + 1));

    analog = lpart | cpart | mpart;
    //analog = analog + offset;
    //analog = (analog*scale);

    return analog;

}




bool Accessory::decodeBit(uint8_t byte, uint8_t bit, bool activeLow) {
    if (byte > 5)
        return false;
    uint8_t swb = _dataarray[byte];
    uint8_t sw = (swb >> bit) & 0x01;
    return activeLow ? (!sw) : (sw);
}



void Accessory::begin() {
    myWire.begin();

    initBytes();

    delay(100);
    _burstRead();
    delay(100);
    _burstRead();

}

void Accessory::_burstRead() {
    _burstReadWithAddress(0);
}


void Accessory::_burstReadWithAddress(uint8_t addr) {
    int readAmnt = sizeof(_dataarray);

    // send conversion command
    myWire.beginTransmission(I2C_ADDR);
    myWire.write(addr);
    myWire.endTransmission();

    // wait for data to be converted
    delay(1);

    // read data
    myWire.readBytes(_dataarray,
                     myWire.requestFrom(I2C_ADDR, sizeof(_dataarray)));

    
    if(_encrypted) {
        for (int i=0; i<sizeof(_dataarray); i++) _dataarray[i] = decryptByte(_dataarray[i],addr+i);
    }
    
    //Serial.print("R ");//Serial.print(addr,HEX);
    //Serial.print(" (");//Serial.print(readAmnt);
    //Serial.print("):\t");
    //printInputs(Serial);
}

void Accessory::_writeRegister(uint8_t reg, uint8_t value) {
        //Serial.print("W ");
                //Serial.print(reg,HEX);
                        //Serial.print(": ");
                                //Serial.println(value,HEX);
    myWire.beginTransmission(I2C_ADDR);
    myWire.write(reg);
    myWire.write(value);
    myWire.endTransmission();
}

void Accessory::_burstWriteWithAddress(uint8_t addr,uint8_t* arr,uint8_t size) {
        //Serial.print("W ");
        //Serial.print(addr,HEX);
        //Serial.print(": ");
        //for (int i=0; i<size; i++) {//Serial.print(arr[i],HEX);//Serial.print(" ");
        //}
        //Serial.println("");
        
    myWire.beginTransmission(I2C_ADDR);
    myWire.write(addr);
    for (int i=0; i<size; i++) myWire.write(arr[i]);
    myWire.endTransmission();
}

void Accessory::enableEncryption(bool enc) {
    _encrypted = enc;
}

void Accessory::printMaps(Stream& stream) {
    stream.println("Listing all mappings");
    if (firstMap==0) {
        stream.println("\tNo Maps");
        return;
    }
    Mapping* c=firstMap;
    do {
        
        stream.print("\t");
        c->printMap(stream);
        c=c->next;
        //Serial.println((uint32_t)c);
    } while (c!=0);
}

void Accessory::_applyMaps() {
    if (firstMap==0) return;
    
    Mapping* c=firstMap;
    

    do {
        c->update();
        c=c->next;
    } while (c!=0);


}
int Accessory::smap(int16_t val, int16_t aMax, int16_t aMid, int16_t aMin, int16_t sMax, int16_t sZero, int16_t sMin) {
    int mapv=sZero;
    if (val>aMid) {
        mapv= map(val,aMid,aMax,sZero,sMax);
    } else if (val<aMid) {
        mapv = map(val,aMin,aMid,sMin,sZero);
    }
    //Serial.print(val);Serial.print(" ");Serial.println(mapv);
    
    return mapv;
}



uint8_t Accessory::addMap(Mapping* m) {
    if (m==0) return 255;
    m->controller = this;
    m->next = 0;
    if (firstMap==0) {
        firstMap = m; // its the first one.
        return 0;
    } else { // walk to end of list
        Mapping* c=firstMap;
        int count=0;
        while(c->next != 0) {
            c = c->next;
            count++;
        }
        // c is end of list
        c->next = m;
        return count;
    }

}

uint8_t Accessory::decryptByte(uint8_t byte, uint8_t address) {
    //return (byte ^ _key_table_1[address % 8]) + _key_table_1[(address % 8)+0x08];
    return (byte ^ 0x97) + 0x97;
}


Accessory::Mapping::Mapping() {
}



Accessory::Mapping::Mapping(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) {
    addServo(chan,max,zero,min);
}

Accessory::Mapping::Mapping(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min,uint16_t cooldown) {
    addServo(chan,max,zero,min);
    _cooldown=cooldown;
}

void Accessory::Mapping::printMap(Stream& stream) {
    stream.print("\t[");
    stream.print(channel);
    stream.print("] \t(");
    stream.print(servoMax);
    stream.print(" *");
    stream.print(servoZero);
    stream.print("* ");
    stream.print(servoMin);
    stream.println(" )");
}

unsigned int Accessory::Mapping::mapVar() {

    return 0;
}

void Accessory::Mapping::update() {
    int var = mapVar();
    
    
    if (_cooldown && var!=servoZero){
      _cooldownCount=_cooldown+millis();
      servo.write(var);
      }

    if (_cooldownCount < millis()) servo.write(mapVar());

}

void Accessory::Mapping::addServo(uint8_t chan,uint8_t max,uint8_t zero,uint8_t min) {
    servo;
    servo.attach(chan);
    channel=chan;
    servoMin=min;
    servoMax=max;
    servoZero=zero;
}




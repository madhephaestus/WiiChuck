#include "Arduino.h"
#include "RfidDb.h"

// Magic number to verify RFID database in EEPROM
#define RFID_DB_MAGIC 0x75

// returns the EEPROM location of the number of items in the database
#define countOffset() (_eepromOffset + 1)

// returns the EEPROM location of the first id in the database
#define firstIdOffset() (countOffset() + 1)

// returns the EEPROM location of the Ith id in the database
#define idOffset(I) (firstIdOffset() + ((I) * sizeof(uint32_t)))

// returns the EEPROM location of the first name in the database
#define firstNameOffset() (idOffset(_maxSize))

// returns the EEPROM location of the Ith name in the database
#define nameOffset(I) (firstNameOffset() + (I) * _maxNameLength)

RfidDb::RfidDb(uint8_t maxSize, uint16_t eepromOffset) {
  init(maxSize, eepromOffset, 0);
}

RfidDb::RfidDb(uint8_t maxSize, uint16_t eepromOffset, uint8_t maxNameLength) {
  init(maxSize, eepromOffset, maxNameLength);
}

void RfidDb::begin() {
  if (!hasMagic()) {
    initDb();
  }
}

uint8_t RfidDb::maxSize() {
  return _maxSize;
}

uint8_t RfidDb::maxNameLength() {
  return _maxNameLength;
}

uint8_t RfidDb::count() {
  return EEPROM.read(countOffset());
}

bool RfidDb::insert(uint32_t id) {
  return insert(id, "");
}

bool RfidDb::insert(uint32_t id, const char* name) {
  // if already exists in the database, we update the name
  int16_t pos = posOf(id);
  if (pos != -1) {
    writeName(pos, name);
    return true;
  }
  uint8_t c = count();
  // no room in database, return false
  if (c >= _maxSize) {
    return false;
  }
  // we know there is room for the id, write in last position and update count
  writeId(c, id);
  writeName(c, name);  
  EEPROM.write(countOffset(), c + 1);
  return true;
}

void RfidDb::remove(uint32_t id) {
  uint8_t originalCount = count();  
  if (originalCount == 0) {
    return;
  }
  int16_t posToRemove = posOf(id);
  if (posToRemove == -1) {
    return;
  }
  uint8_t newCount = originalCount - 1;
  if (newCount > 0 || newCount == posToRemove) {
    uint32_t idToMove = getId(newCount);
    writeId(posToRemove, idToMove);
    copyName(newCount, posToRemove);
  }
  EEPROM.write(countOffset(), newCount);
}

bool RfidDb::getId(uint8_t pos, uint32_t &id) {
  if (pos >= count()) {
    return false;
  }
  id = getId(pos);
  return true;
}

bool RfidDb::getName(uint8_t pos, char* name) {
  if (pos >= count() || _maxNameLength == 0) {
    return false;
  }
  uint16_t base = nameOffset(pos);
  for (int i = 0; i < _maxNameLength; i++) {
    name[i] = EEPROM.read(base + i);
    if (name[i] == '\0') {
      break;
    }
  }
  return true;
}

bool RfidDb::contains(uint32_t id) {
  return posOf(id) != -1;
}

bool RfidDb::contains24(uint32_t id) {
  return posOf24(id) != -1;
}

// Returns the position of the given id in the database or -1 if the id is 
// not in the database
int16_t RfidDb::posOf(uint32_t id) {
  return posOf(id, 0xFFFFFFFF);
}

// Returns the position of the given id in the database when compared on the
// low 24 bits of the id.
int16_t RfidDb::posOf24(uint32_t id) {
  return posOf(id, 0x00FFFFFF);
}

// Returns the position of the given id when compared with ids in the datbase
// after both the database id and the given id are bit masked with the given
// mask
int16_t RfidDb::posOf(uint32_t id, uint32_t mask) {
  uint32_t maskedId = id & mask;
  for (uint8_t i = 0, n = count(); i < n; i++) {
    if (maskedId == (getId(i) & mask)) {
      return i;
    }
  }
  return -1;
}

// Returns the id at the given position
uint32_t RfidDb::getId(uint8_t pos) {
  uint32_t id;
  EEPROM.get(idOffset(pos), id);
  return id;
}

// Writes an id to the database at a given position
inline void RfidDb::writeId(uint8_t pos, uint32_t id) {
  EEPROM.put(idOffset(pos), id);
}

// Writes a name to the database at a given position
void RfidDb::writeName(uint8_t pos, const char* name) {
  if (_maxNameLength > 0) {
    uint8_t base = nameOffset(pos);
    for (int i = 0; i < _maxNameLength - 1; i++) {
      EEPROM.write(base + i, name[i]);
    }
    // Ensure we null terminate
    EEPROM.write(base + _maxNameLength - 1, '\0');
  }
}

void RfidDb::copyName(uint8_t srcPos, uint8_t destPos) {
  if (_maxNameLength > 0) {
    uint16_t srcbase = nameOffset(srcPos);
    uint16_t destBase = nameOffset(destPos);
    for (int i = 0; i < _maxNameLength; i++) {
      char c = EEPROM.read(srcbase + i);
      EEPROM.write(destBase + i, c);
      if (c == '\0') {
        break;
      }
    }
  }
}

// Returns whether the EEPROM location at the EEPROM base address
// contains the magic number
bool RfidDb::hasMagic() {
  return EEPROM.read(_eepromOffset) == RFID_DB_MAGIC;
}

// Initialises the database by writing the magic number to the base
// EEPROM address, followed by a zero count.
void RfidDb::initDb() {
  EEPROM.write(_eepromOffset, RFID_DB_MAGIC);
  EEPROM.write(countOffset(), 0);
}

void RfidDb::init(uint8_t maxSize, uint16_t eepromOffset, uint8_t maxNameLength) {
  _maxSize = maxSize;
  _eepromOffset = eepromOffset;
  _maxNameLength = maxNameLength;
}

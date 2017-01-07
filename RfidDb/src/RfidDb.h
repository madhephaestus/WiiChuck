#ifndef RFID_DB_H
#define RFID_DB_H

#include "Arduino.h"
#include "EEPROM.h"

// A database designed to store a number of RFIDs of 32 bits up to a fixed
// size. The database is stored in EEPROM and requires (N * 4) + 2 bytes of 
// storage where  is the maximum number of entries.
//
// Identifiers can be added, removed and checked for existence.
//
// Performance of insert, remove and contains is O(N).
// Performance of get at index is O(1)
class RfidDb {
  public:
    // Creates an RFID database which does not store names.
    // Parameters:
    //   maxSize: The maximum number of ids that the database can hold
    //   eepromOffset: the byte offset from 0 where the databse resides
    //       in EEPROM
    RfidDb(uint8_t maxSize, uint16_t eepromOffset);

    // Creates an RFID database which does not store names.
    // Parameters:
    //   maxSize: The maximum number of ids that the database can hold
    //   eepromOffset: the byte offset from 0 where the databse resides
    //       in EEPROM
    //   maxNameSize: The maximum number of bytes (including null terminator)
    //       of names that are stored with Ids.
    RfidDb(uint8_t maxSize, uint16_t eepromOffset, uint8_t maxNameSize);

    // Initialises the database in EEPROM if the location at EEPROM
    // offset does not contain the magic number.
    void begin();

    // Returns the maximum number of identifiers that the database can
    // contain.
    uint8_t maxSize();

    // Returns the maximum length of name that can be stored, including null
    // terminator
    uint8_t maxNameLength();

    // Returns the number of identifiers currently in the database.
    uint8_t count();

    // Inserts the identifier in the database with an empty name. If the
    // identifier already exists then the name is replace with a null string.
    // Returns true if the indentifier was successfully inserted or already
    // existed in the database. Returns false if the database is at maximum
    // capacity.
    bool insert(uint32_t id);

    // Inserts the identifier in the database with the specified name. If the
    // given name is longer than the specified maximum name length then
    // the name is truncated to fit. If the identifier already exists then
    // the name is updated to the given name.
    bool insert(uint32_t id, const char* name);

    // Removes the entry with the given identifier from the database, if it
    // exists. 
    void remove(uint32_t id);

    // Returns the identfier at the given position. Callers should check
    // the return value before using the identifier. Returns true if
    // the position is less than the count and writes the identifier value
    // at the given address.
    // Returns false if pos >= count
    bool getId(uint8_t pos, uint32_t &id);

    // Returns the name at the given position. Callers should check
    // the return value before using the name. Returns true if
    // the position is less than the count and writes the identifier value
    // to the given string.
    // Callers should allocate a string of at least maxNameSize bytes
    // Returns false if pos >= count or names are not stored in the database
    bool getName(uint8_t pos, char* name);

    // Returns whether the database contains the given identifier.
    bool contains(uint32_t id);

    // Returns whether the database contains an indentifier where the low 24
    // bits matches the low 24 bits of the given identifier. This is useful
    // when reading from a Wiegand 26 device (that returns a 24 bit id) with
    // identifers stored as 32 vit ids.
    bool contains24(uint32_t id);

  private:
    uint16_t _eepromOffset;
    uint8_t _maxSize;
    uint8_t _maxNameLength;

    int16_t posOf(uint32_t id);
    int16_t posOf24(uint32_t id);
    int16_t posOf(uint32_t id, uint32_t mask);
    uint32_t getId(uint8_t pos);
    void writeId(uint8_t pos, uint32_t id);
    void writeName(uint8_t pos, const char* name);
    void copyName(uint8_t srcPos, uint8_t destPos);
    bool hasMagic();
    void initDb();
    void init(uint8_t maxSize, uint16_t eepromOffset, uint8_t maxNameSize);
};

#endif

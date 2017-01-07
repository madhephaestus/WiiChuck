# RfidDb
RfidDb provides a database that contains 32 bit identifiers and optionally
associated names which can be used with an RFID reader to control access. 
The identifiers and names are stored in EEPROM and will persist across
restarts.

## Creating and initialising RfidDb
An RfidDb must be created with the maximum size of the database and the
EEPROM location at which the database starts. Optionally, the maximum
length of the name field can be specified in the constructor. If this
is not specified (or is zero), then then storage of names will be disabled.
The name length includes a null terminating character.

To find the amount of EEPROM required for the database, add 4 to the maximum
length of the name, multiply by the maximum number of entries and the add
two bytes. For example, a 16 entry database with 8 character names takes 194
bytes; ((4 + 8) * 16) + 2.

The database must be initialised by calling the begin() method prior to use.
This is generally done in the setup method.

To create a database that can hold 16 identifiers, with storage starting at
byte 0 of EEPROM without storing names:
```c++
RfidDb db = RfidDb(16, 0);

void setup() {
  db.begin();
}
```

To create a database that can hold 8 identifiers, with storage starting at
byte 25 of EEPROM, with a maximum of 8 character
names:
```c++
RfidDb db = RfidDb(8, 24, 8);

void setup() {
	db.begin();
}
```

## Database Methods
The public methods available on the RfidDb are described below.

### maxSize
Returns the maximum number of identifiers that can be held in the database.

```c++
db.maxSize();
```

### maxNameLength
Returns the maximum length of the name field (including the terminating null)

```c++
db.maxNameLength();
```

### count
Returns the number of identifiers currently in the database.

```c++
db.count();
```

### insert
Inserts a 32 bit identifier and optionally a name into the database.
Returns true if the insert was successful, or the identifier already existed
in the database and false if the identifier could not be inserted due to a
full database.

Names which are longer than the maximum name length are truncated to fit
in the database. If the database is not storing names and a name is stored,
then the name is silently ignored. If no name is stored and the database
stores names, then the name is set to the null string.

To insert an id only:
```c++
if (db.insert(10042)) {
  Serial.println("Inserted or already existed");
} else {
  Serial.println("Insert failed");
}
```

To insert an id and name:
```c++
if (db.insert(10042, "ABCD")) {
  Serial.println("Inserted or already existed");
} else {
  Serial.println("Insert failed");
}
```

### remove
Removes an entry identifed by the given 32 bit identifier from the database.
If the database is empty or thee identifier was not in the database then this 
method does nothing.

```c++
db.remove(10042);
```

### getId
Returns the identifier at the specified zero-based position. The caller must
provide an unsigned 32-bit integer which this function will populate with
the identifier value. The method returns true if the get was successful and
false if the get was unsuccessful due to the postion being out of range.

```c++
uint32_t id;
if (db.get(4, id)) {
  Serial.print("Get successful. id = ");
  Serial.println(id);
} else {
  Serial.println("Get unsuccessful");
}
```

### getName
Returns the name at the specified zero based position. The caller must supply
a char[] to receive the name of at least the maximum name length of the
database. The method returns true if the database stores name and the
supplied postion is in range (between 0 and count() - 1). The method returns
false if the database does not store names or the positon is out of range.

```c++
char name[db.maxNameLength()];
if (db.get(4, name)) {
	Serial.print("Get successful. name = ");
	Serial.println(name);
} else {
	Serial.println("Get unsuccessful");
}
```

### contains
Returns true if the database contains the given identifier, false otherwise.

```c++
if (db.contains(10042)) {
  Serial.println("In database");
} else {
  Serial.println("Not in database");
}
```

## contains24
Returns 2 if the low order 24-bits of the specified id match the low order
24 bits of any id in the databse. This us useful if 32 bit ids are stored in
the database and the ids are read using a Wiegand26 reader which returns
24 bits.

```c++
if (db.contains24(10042)) {
  Serial.println("In database");
} else {
  Serial.println("Not in database");
}
```
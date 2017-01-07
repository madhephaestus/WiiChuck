/*
  Ingenia_SerialServoDrive.h - Library for control Ingenia Servo Drives.
*/

#ifndef INGENIA_SERIALSERVODRIVE_h
#define INGENIA_SERIALSERVODRIVE_h

#include "Arduino.h"

#define MAX_MSG_LEN   40

class Ingenia_SerialServoDrive
{
public:
    //! The enumeration of Drive Modes
    typedef enum 
    {
        DRIVE_MODE_PROFILE_POSITION = 1,            //!< Profile position mode
        DRIVE_MODE_PROFILE_VELOCITY = 3,            //!< Profile velocity mode
        DRIVE_MODE_PROFILE_TORQUE = 4,              //!< Profile torque mode
        //DRIVE_MODE_VELOCITY = 2,                  //!< Velocity mode
        DRIVE_MODE_HOMING = 6,                      //!< Homing mode
        DRIVE_MODE_INTERPOLATED_POSITION = 7,       //!< Interpolated position mode
        DRIVE_MODE_CYCLIC_SYNC_POSITION = 8,        //!< Cyclic sync position mode
        DRIVE_MODE_CYCLIC_SYNC_VELOCITY = 9,        //!< Cyclic sync velocity mode
        DRIVE_MODE_CYCLIC_SYNC_TORQUE = 10,         //!< Cyclic sync torque mode
        DRIVE_MODE_OPEN_LOOP_SCALAR = -1,           //!< Open loop scalar mode
        DRIVE_MODE_OPEN_LOOP_VECTOR = -2            //!< Open loop vector mode
    } EDriveModes;  

    Ingenia_SerialServoDrive(Stream * virtualPort);
    Ingenia_SerialServoDrive(Stream * virtualPort, uint8_t u8NodeID);

    uint32_t read(uint32_t u32Index, uint8_t u8SubIndex);
    uint32_t read(uint32_t u32Index, uint8_t u8SubIndex, bool &bIsValid);

    void write(uint32_t u32Index, uint8_t u8SubIndex, uint64_t value);
    void write(uint32_t u32Index, uint8_t u8SubIndex, int64_t value);
    void write(uint32_t u32Index, uint8_t u8SubIndex, uint32_t value);
    void write(uint32_t u32Index, uint8_t u8SubIndex, int32_t value);
    void write(uint32_t u32Index, uint8_t u8SubIndex, uint16_t value);
    void write(uint32_t u32Index, uint8_t u8SubIndex, int16_t value);
    void write(uint32_t u32Index, uint8_t u8SubIndex, uint8_t value);
    void write(uint32_t u32Index, uint8_t u8SubIndex, int8_t value);

    // Binary enable/disable functions
    void enableBinary();
    void disableBinary();

    // Motor enable/disable functions
    void enableMotor();
    void disableMotor();

    // Homing
    void doHoming(int8_t i8HomingMethod);

    // Motion funcions
    void setModeOfOperation(EDriveModes driverMode);

    void setTargetVelocity(int32_t value);
    void setTargetTorque(int16_t value);
    void setTargetPosition(int32_t value);

    int32_t getActualPosition();
    int32_t getActualVelocity();
    int16_t getActualTorque();


    // Statusword functions
    uint16_t getStatusword();
    bool statuswordIsReadyToSwitchOn();
    bool statuswordIsSwitchedOn();
    bool statuswordIsOperationEnabled();
    bool statuswordIsFault();
    bool statuswordIsVoltageEnabled();
    bool statuswordIsQuickStop();
    bool statuswordIsSwitchOnDisabled();
    bool statuswordIsWarning();
    bool statuswordIsRemote();
    bool statuswordIsTargetReached();
    bool statuswordIsInternalLimitActive();
    bool statuswordIsInitialAngleDeterminationProcessFinished();

    // Homing status functions
    bool homingStatusIsInProgress();
    bool homingStatusIsError();
    bool homingStatusIsSuccess();
    bool homingStatusIsAttained();
    bool homingStatusIsInterrupted();
    bool homingStatusNotStarted();

private:
    void write(uint8_t u8Node, uint32_t u32Index, uint8_t u8SubIndex, uint32_t u32ObjSize, uint64_t value);
    void write(uint32_t u32Index, uint8_t u8SubIndex, uint32_t u32ObjSize, uint64_t value);
    
    void setTargetPosition(int32_t value, bool isImmediate, bool isRelative, bool isHaltEnabled);

    Stream * _virtualPort;
    bool _isBinaryEnabled = false;
    char serial_buf[MAX_MSG_LEN];
    uint8_t _u8Node = 0x20;

    // Types of access to registers
    typedef enum  
    { 
        READ = 0,               //!< Read access to register (read data)
        WRITE                   //!< Write access to register (send data)
    } ERegAccess;

    // Multiple access for a region of memory of 64 bits
    typedef union
    {
        int64_t   i64;            //!< As one 64 integer
        uint64_t  u64;            //!< As one 64 unsigned integer
        int32_t   i32[2];         //!< As two 32 integer
        uint32_t  u32[2];         //!< As two 32 unsigned integer
        int16_t   i16[4];         //!< As four 16 integers
        uint16_t  u16[4];         //!< As four 16 unsigned integers
        int8_t    i8[8];          //!< As eight 8 integers
        uint8_t   u8[8];          //!< As eight 8 unsigned integers
        unsigned char   b8[8];    //!< As eight 8 bytes
    } t_int64;

    // Message data structure
    typedef struct 
    {
        unsigned char b8ID;             //!< ID of motion controller
        uint32_t u32Register;           //!< Register to access
        ERegAccess eRegAccess;          //!< Acces type (read or write)
        unsigned char b8DataLength;     //!< Data length for write access
        t_int64 ti64Data;               //!< Data 
    } TMessage, *PTMessage;

    int serial_read(unsigned long timeout);
    void serial_write(char* strMessage, size_t sLength);
    bool encodeMsg(TMessage *pMsg, char* strMessage, size_t *pubLenght, bool isBinary);
    bool decodeMsg(const char* strMessage, TMessage *pMsg, bool isBinary);

    //! State Machine Commands
    typedef enum
    {
      COMMAND_SHUTDOWN,                           //!< Shutdown command
      COMMAND_SWITCH_ON,                          //!< Switch on command
      COMMAND_SWITCH_ON_AND_ENABLE_OPERATION,     //!< Switch on + Enable operation command
      COMMAND_DISABLE_VOLTAGE,                    //!< Disable voltage command
      COMMAND_QUICK_STOP,                         //!< Quick stop command
      COMMAND_DISABLE_OPERATION,                  //!< Disable operation command
      COMMAND_ENABLE_OPERATION,                   //!< Enable operation command
      COMMAND_FAULT_RESET                         //!< Fault reset command
    } EStateMachineCommand;

    //! The enumeration of State Machine Status
    typedef enum
    {
        STATUS_NOT_READY_TO_SWITCH_ON,      //!< Not ready to switch ON 
        STATUS_SWITCH_ON_DISABLED,          //!< Switch ON disabled
        STATUS_READY_TO_SWITCH_ON,          //!< Ready to switch ON
        STATUS_SWITCH_ON,                   //!< Switch ON status
        STATUS_OPERATION_ENABLED,           //!< Operation enabled
        STATUS_QUICK_STOP_ACTIVE,           //!< Quick stop active
        STATUS_FAULT_REACTION_ACTIVE,       //!< Fault reaction active
        STATUS_FAULT,                       //!< Fault status
        STATUS_UNKNOWN                      //!< Unknown status
    } EStateMachineStatus;

    // State machine
    // 
    bool _isInitialAngleDeterminationProcessFinished;
    void sendStateMachineCommand(const EStateMachineCommand eStateMachineCommand);
    void setStateMachineStatus(const EStateMachineStatus eNewStateMachineStatus);
    EStateMachineStatus decodeStatusWord(uint16_t u16StatusWord);
    void getStateMachineStatus(EStateMachineStatus* peStateMachineStatus);
    void goToStatusFrom(const EStateMachineStatus eCurrentStateMachineStatus, const EStateMachineStatus eDestinationStateMachineStatus);
    bool verifyStatusIsReached(const EStateMachineStatus eNextStateMachineStatus);

};

#endif


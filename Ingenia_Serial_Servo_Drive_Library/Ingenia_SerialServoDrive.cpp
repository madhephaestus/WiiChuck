/*
    Ingenia_SerialServoDrive.cpp - Ingenia serial servo drives master library for Arduino.
*/
#include "Ingenia_SerialServoDrive.h"
#include <stdlib.h>

#define RS232_CRCHAR  0x0D
#define MAX_NUM_MC    127     //!< Maximum number of posible motion controllers connected to a single channel

Ingenia_SerialServoDrive::Ingenia_SerialServoDrive(Stream * virtualPort)
{
    this->_virtualPort = virtualPort;
}

Ingenia_SerialServoDrive::Ingenia_SerialServoDrive(Stream * virtualPort, uint8_t nodeID)
{
    this->_virtualPort = virtualPort;
    this->_u8Node = nodeID;
}

uint32_t Ingenia_SerialServoDrive::read(uint32_t u32Index, uint8_t u8SubIndex)
{
    uint32_t u32Message;
    bool isValidRead = false;

    while (!isValidRead)
    {
        u32Message = read(u32Index, u8SubIndex,isValidRead);
    }

    return u32Message;
}

uint32_t Ingenia_SerialServoDrive::read(uint32_t u32Index, uint8_t u8SubIndex, bool &bIsValid)
{
    uint32_t u32Result = 0;
    PTMessage decodedMessage = new TMessage;
    TMessage tMessage;
    tMessage.b8ID = _u8Node;   
    tMessage.u32Register = ((uint32_t) u8SubIndex << 16) | u32Index;
    tMessage.eRegAccess = READ;  
    tMessage.b8DataLength = 0;

    size_t eMsgLen;
    char* eMsgStr = new char[MAX_MSG_LEN];
    encodeMsg(&tMessage, eMsgStr, &eMsgLen, this->_isBinaryEnabled);

    // Flush port
    while (this->_virtualPort->available())
        this->_virtualPort->read();

    serial_write(eMsgStr, eMsgLen);

    bool isValid = false;
    unsigned long long started_waiting_at = millis();
    while(!isValid)
    {
        // Check if timeout excedeed
        if (millis() - started_waiting_at > 1000 )
        {
            break;
        }

        if (serial_read(200) != 0)
        {
            decodeMsg((const char*)&serial_buf, decodedMessage, _isBinaryEnabled);
            if (decodedMessage->u32Register == tMessage.u32Register
                && decodedMessage->eRegAccess == WRITE
                && decodedMessage->b8ID == tMessage.b8ID)
            {
                isValid = true;
            }

            if (!isValid) serial_write(eMsgStr, eMsgLen);
        }
    }

    if (isValid)
        u32Result = decodedMessage->ti64Data.u32[0];
    
    bIsValid = isValid;
    
    delete(eMsgStr);
    delete(decodedMessage);
    return u32Result;
}

void Ingenia_SerialServoDrive::write(uint32_t u32Index, uint8_t u8SubIndex, uint64_t value)
{
    write(u32Index, u8SubIndex, 8, value);
}

void Ingenia_SerialServoDrive::write(uint32_t u32Index, uint8_t u8SubIndex, int64_t value)
{
    write(u32Index, u8SubIndex, 8, value);
}

void Ingenia_SerialServoDrive::write(uint32_t u32Index, uint8_t u8SubIndex, uint32_t value)
{
    write(u32Index, u8SubIndex, 4, value);
}

void Ingenia_SerialServoDrive::write(uint32_t u32Index, uint8_t u8SubIndex, int32_t value)
{
    write(u32Index, u8SubIndex, 4, value);
}

void Ingenia_SerialServoDrive::write(uint32_t u32Index, uint8_t u8SubIndex, uint16_t value)
{
    write(u32Index, u8SubIndex, 2, value);
}

void Ingenia_SerialServoDrive::write(uint32_t u32Index, uint8_t u8SubIndex, int16_t value)
{
    write(u32Index, u8SubIndex, 2, value);
}


void Ingenia_SerialServoDrive::write(uint32_t u32Index, uint8_t u8SubIndex, uint8_t value)
{
    write(u32Index, u8SubIndex, 1, value);
}

void Ingenia_SerialServoDrive::write(uint32_t u32Index, uint8_t u8SubIndex, int8_t value)
{
    write(u32Index, u8SubIndex, 1, value);
}

void Ingenia_SerialServoDrive::write(uint32_t u32Index, uint8_t u8SubIndex, uint32_t u32ObjSize, uint64_t value)
{
    this->write(_u8Node, u32Index, u8SubIndex, u32ObjSize, value);
}

void Ingenia_SerialServoDrive::write(uint8_t u8Node, uint32_t u32Index, uint8_t u8SubIndex, uint32_t u32ObjSize, uint64_t value)
{
    uint32_t u32Register;
    t_int64 i64DataValue;
    switch(u32ObjSize)
    { 
        case 1:
            i64DataValue.i8[0] = (int8_t) value;
            break;
        case 2:
            i64DataValue.i16[0] = (int16_t) value;
            break;
        case 4:
            i64DataValue.i32[0] = (int32_t) value;
            break;
        case 8:
            i64DataValue.i64 = (int64_t) value;
            break;
        default:
            break;
    }

    u32Register = ((uint32_t)u8SubIndex << 16) | u32Index;

    // Prepare message to send
    TMessage tMessage;
    tMessage.b8ID = u8Node;   
    tMessage.u32Register = u32Register;    
    tMessage.eRegAccess = WRITE;  
    tMessage.b8DataLength = u32ObjSize;
    tMessage.ti64Data = i64DataValue;

    // Send message thorugh interface
    size_t eMsgLen;
    char* eMsgStr = new char[MAX_MSG_LEN];
    encodeMsg(&tMessage, eMsgStr, &eMsgLen, this->_isBinaryEnabled);
    serial_write(eMsgStr, eMsgLen);
    delete(eMsgStr);
}

// Binary Defines

#define MIN_BIN_FRAME_SIZE          13
#define NUMBER_OF_DATA_FIELD_SIZE    2
#define NUMBER_OF_DATA_HIGH         11
#define NUMBER_OF_DATA_LOW          12
#define MAX_DATA_FIELD_SIZE          8

#define DEFAULT_TIMEOUTS  1000

#define SYNC_BYTE_BIN       0x55        //!< synchronization byte Bin Mode
#define NUM_SYNC_BYTE       4           //!< Number of Synchronization bytes

int Ingenia_SerialServoDrive::serial_read(unsigned long timeout)
{
    uint16_t u16NumberOfData = 0;
    uint8_t u8EndMsgCounter = 0;
    uint8_t u8Counter = 0;
    bool isMessageCompleted = false;
    unsigned long long started_waiting_at = millis();

    while (!isMessageCompleted)
    {
        uint8_t b8RXBuff = 0;

        // Check if timeout excedeed
        if (millis() - started_waiting_at > timeout )
        {
            break;
        }

        // Check if there are bytes waiting for read
        if (this->_virtualPort->available())
        {
            this->_virtualPort->readBytes(&b8RXBuff,1);
        }
        else
        {
            continue;
        }

        if (!_isBinaryEnabled)
        {
            if (!b8RXBuff) // avoid null characters
            {
                continue;
            }
        }

        // if message lenght excedeed
        if (u8Counter >= MAX_MSG_LEN)
        {
            break;
        }

        serial_buf[u8Counter] = b8RXBuff;


        if (_isBinaryEnabled)
        {
            /* Fill buffer until it have the minimum expected size. Minimum size: 13 */
            /* ---------------------------------------------------------------------- */
            /*| Address | Function | MEI type | Prot. Control | Res. Filed | Node ID |*/
            /*|   1     |    1     |     1    |       1       |      1     |     1   |*/
            /* ---------------------------------------------------------------------- */
            /* -------------------------------------------------------------------    */
            /*| Index | Subindex | Starting Address | Number of data | Data | CRC |   */
            /*|   2   |    1     |         2        |        2       |   0  |  0  |   */
            /* -------------------------------------------------------------------    */
            /* Buffer is filled until Starting Address field at least         */
            if (u8Counter > (MIN_BIN_FRAME_SIZE - NUMBER_OF_DATA_FIELD_SIZE)) // TODO: Add defines
            {
                /* Save the high byte of the number of data field*/
                if (u8Counter == NUMBER_OF_DATA_HIGH) // TODO: Add defines
                {
                    u16NumberOfData = (uint16_t)(b8RXBuff << 8) & 0xFF00;
                }
                /* Save the low byte of the number of data field*/
                else if (u8Counter == NUMBER_OF_DATA_LOW) // TODO: Add defines
                {
                    u16NumberOfData = (uint16_t)(b8RXBuff) & 0x00FF;
                    if (u16NumberOfData < MAX_DATA_FIELD_SIZE + 1)
                    {
                        /* Add the CRC field size to the number of data to be received from
                            this point */
                        /* if CRC enable
                        u16NumberOfData += 2;
                        */
                    }
                    /* if number of data is outside of the expeted value,
                    * the packet is droped. */
                    else
                    {
                        u16NumberOfData = 0;
                    }
                }
                else
                {
                    /* if frame have all data, wait until four consecutive sync bytes 
                        * are received */
                    /* It considers that all data has been received when the frame size
                        * is equal to Minimum size + number of data inside data field + CRC
                        *  if it is  available */
                    if (b8RXBuff == SYNC_BYTE_BIN && (u8Counter >= (u16NumberOfData + (uint32_t) MIN_BIN_FRAME_SIZE))) // Binary mode ends with 0x55
                    {
                        u8EndMsgCounter++;
                        if (u8EndMsgCounter == NUM_SYNC_BYTE)
                        {
                            // Message reception completed
                            // Decode message and write it into TMessage structure
                            isMessageCompleted = true;
                            continue;
                        }
                    }
                    /* Fill buffer if there are more bytes in the data field*/
                    else
                    {
                        u8EndMsgCounter = 0;
                    }
                }
            }
        }
        else
        {
            if (b8RXBuff == RS232_CRCHAR)
            {
                serial_buf[u8Counter] = 0;
                isMessageCompleted = true;
                continue;
            }
        }
        u8Counter++;
    }

    if (isMessageCompleted){
        return u8Counter;
    }
    else return 0;
}

void Ingenia_SerialServoDrive::serial_write(char* strMessage, size_t sLength)
{
    this->_virtualPort->write(strMessage,sLength);
}

#define MCL_MSG_MAXDATALENGTH       8

bool Ingenia_SerialServoDrive::encodeMsg(TMessage *pMsg, char* strMessage, size_t *pubLenght, bool isBinary)
{
    unsigned char b8NodeID;
    uint32_t u32Register;
    uint8_t u8DataLength;
    t_int64 ti64Data;
    uint8_t u8I;
    //
    // Verify parameters
    //
    if(pMsg == NULL || strMessage == NULL)
    {
        return false;
    }
    //
    // Start encoding message process
    //
    b8NodeID = pMsg->b8ID;
    u32Register = pMsg->u32Register;
    u8DataLength = pMsg->b8DataLength;

    if(isBinary)
    {
        // CRC: uint16_t u16CRC;
        uint8_t u8Index = 0, u8i;

        strMessage[u8Index++] = pMsg->b8ID; /* ModBus Address */
        strMessage[u8Index++] = 43;                                 /* ModBus Function */
        strMessage[u8Index++] = 13;                                 /* ModBus MEI */
        strMessage[u8Index++] = pMsg->eRegAccess;       /* Protocol control */
        strMessage[u8Index++] = 0;                                  /* reserved Field */
        strMessage[u8Index++] = pMsg->b8ID; /* Node ID */
        strMessage[u8Index++] = (u32Register >> 8) & 0xFF;
        strMessage[u8Index++] = (u32Register) & 0xFF;
        strMessage[u8Index++] = (u32Register >> 16) & 0xFF;
        strMessage[u8Index++] = 0;                                  /* Starting Address */
        strMessage[u8Index++] = 0;                                  /* Starting Address */
        strMessage[u8Index++] = 0;                                   /* Number of data */
        strMessage[u8Index++] = u8DataLength;
        /* Number of data */
        if(pMsg->eRegAccess != 0) // Write message
        {
            for(u8i = 0; u8i < u8DataLength; u8i++)
            { /* Data */
                strMessage[u8Index++] = pMsg->ti64Data.u8[u8i]; /* Data */
            }
        }
        /* Compute CRC (Not functional) */
        /*
        if (isUARTCrcEnabled != FALSE)
        {
            u16CRC = CRC16Check(pBuffer, u16Index);
            strMessage[u16Index++] = (u16CRC >> 8) & 0x00FF;
            strMessage[u16Index++] =  u16CRC & 0x00FF;
        }*/

        strMessage[u8Index++] = 0; // CRC (Not Enabled)
        strMessage[u8Index++] = 0; // CRC (Not Enabled)
        
        #define SYNC_BYTE_BIN 0x55

        strMessage[u8Index++] = SYNC_BYTE_BIN;                     /* End of frame */
        strMessage[u8Index++] = SYNC_BYTE_BIN;
        strMessage[u8Index++] = SYNC_BYTE_BIN;
        strMessage[u8Index++] = SYNC_BYTE_BIN;

        *pubLenght = u8Index;

    }
    else
    {

            
        if(pMsg->eRegAccess == READ)
        {
            sprintf(strMessage, "0x%02X %s 0x%lX\r", 
            b8NodeID, "R", u32Register);
        }
        else
        {      
            ti64Data.i64 = 0;
            for(u8I = 0; u8I < u8DataLength; u8I++)
            {
                ti64Data.b8[u8I] = pMsg->ti64Data.b8[u8I];
            }

            switch(u8DataLength)
            {
                case 1:
                    sprintf(strMessage, "0x%02X %s 0x%lX %d\r", 
                        b8NodeID, "W", u32Register, ti64Data.i8[0]);
                    break;
                case 2:
                    sprintf(strMessage, "0x%02X %s 0x%lX %d\r", 
                        b8NodeID, "W", u32Register, ti64Data.i16[0]);
                    break;
                case 4:
                    sprintf(strMessage, "0x%02X %s 0x%lX %ld\r", 
                        b8NodeID, "W", u32Register, ti64Data.i32[0]);
                    break;             
                case 5:
                case 6:
                case 7:
                case 8:

                    sprintf(strMessage, "0x%02X %s 0x%lX %lld\r", 
                        b8NodeID, "W", u32Register, ti64Data.i64);
                    break;

                default:
                    return false;
            }
        }
        *pubLenght = (size_t) strlen(strMessage);
    }
        return true;
}

bool Ingenia_SerialServoDrive::decodeMsg(const char* strMessage, TMessage *pMsg, bool isBinary)
{
    unsigned char b8NodeID;
    uint32_t u32Register;
    char* pb8Token;
    bool bSuccess;

    // Verify parameters
    if(pMsg == NULL || strMessage == NULL)
    {
        return false; 
    }

    // Start decoding message process
    bSuccess = false;
    while(1)
    {
        if(isBinary)
        {
            uint16_t u16Index = 0, u16i;

            pMsg->b8ID = strMessage[u16Index++]; /* ModBus Address */
            u16Index+=2;
            pMsg->eRegAccess = (strMessage[u16Index++] & 0x01) == 0 ? READ : WRITE;
            u16Index++;
            pMsg->b8ID = strMessage[u16Index++];   /* Node ID */
            pMsg->u32Register = ((uint16_t)(strMessage[u16Index] << 8) & 0xFF00) | ((uint16_t)(strMessage[u16Index+1]) & 0x00FF);
            u16Index+=2;
            pMsg->u32Register = (pMsg->u32Register) + ((uint32_t)strMessage[u16Index++] << 16);
            u16Index+=3;
            pMsg->b8DataLength = strMessage[u16Index++];                  /* Number of data */
            if(pMsg->eRegAccess == WRITE)
            {
                pMsg->ti64Data.i64 = 0;
                for(u16i = 0; u16i < pMsg->b8DataLength; u16i++)
                {
                    pMsg->ti64Data.u8[u16i] = strMessage[u16Index++];            /* Data */
                }
            }
            bSuccess = true;
            break;
        }
        else
        {
            pb8Token = NULL;

            // Get the Node ID
            // Look for the first token " "
            if((pb8Token = strtok((char*)strMessage, " ")) == NULL)
            {
                // Node not found in message
                break;
            }    
            b8NodeID = (unsigned char) strtol(pb8Token, NULL, 0);
            if(b8NodeID > MAX_NUM_MC)
            {
                // Out of limits
                break;
            }
            pMsg->b8ID = b8NodeID;

            // Get the register
            if((pb8Token = strtok(NULL, " ")) == NULL)
            {
                // Not found in message
                break;
            }

            // Get access type
            if(*pb8Token == 'R')
            {
                pMsg->eRegAccess = READ;
            }
            else if(*pb8Token == 'W')
            {
                pMsg->eRegAccess = WRITE;
            }
            else
            {
                // Unknown
                break;
            }

            // Get the register
            if((pb8Token = strtok(NULL, " ")) == NULL)
            {
                // Not found in message
                break;
            }
            u32Register = (uint32_t) strtol(pb8Token, NULL, 0);
            pMsg->u32Register = (u32Register & 0xFFFFFF);

            // If the message is a reading the process stops here
            if(pMsg->eRegAccess == READ)
            {
                bSuccess = true;
                break;
            }

            // If the message is a writing, get the parameter
            if((pb8Token = strtok(NULL, " ")) == NULL)
            {
                // Parameter not found in message
                break;
            }

            // Get data length
            pMsg->b8DataLength = MCL_MSG_MAXDATALENGTH;

            // Get data value
            if(strchr(pb8Token, 'x') != NULL)
            {   // Data is in hexadecimal base
                pMsg->ti64Data.u64 = strtoul(pb8Token, NULL, 16);
            }
            else
            {   // Data is in decimal base
                pMsg->ti64Data.u64 = strtoul(pb8Token, NULL, 10);
            }
            bSuccess = true;
            break;
        }
    }

    return bSuccess;
}


//
// Status Word Register (SWR) bits meaning
//
//  ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// |  15      |     14       | 13         12 |      11      |   10    |   9       |      8       |   7     |     6     |    5   |    4    |   3   |    2      |    1     |      0     |
// | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// | Reserved |Init angle    | Operation     | Internal     | Target  | Remote    | Reseved      | Warning | Switch on | Quick  | Voltage | Fault | Operation | Switched |  Ready to  |
// |          |proc finished | mode specific | limit active | reached |           |              |         | disabled  | stop   | enabled |       | enabled   | on       | switch on  |
//  ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
#define STATUS_WORD_REGISTER_BITS_READY_TO_SWITCH_ON     0x0001
#define STATUS_WORD_REGISTER_BITS_SWITCHED_ON            0x0002
#define STATUS_WORD_REGISTER_BITS_OPERATION_ENABLED      0x0004
#define STATUS_WORD_REGISTER_BITS_FAULT                  0x0008
#define STATUS_WORD_REGISTER_BITS_VOLTAGE_ENABLED        0x0010
#define STATUS_WORD_REGISTER_BITS_QUICK_STOP             0x0020
#define STATUS_WORD_REGISTER_BITS_SWITCH_ON_DISABLED     0x0040
#define STATUS_WORD_REGISTER_BITS_WARNING                0x0080
#define STATUS_WORD_REGISTER_BITS_RESERVED               0x0100
#define STATUS_WORD_REGISTER_BITS_REMOTE                 0x0200
#define STATUS_WORD_REGISTER_BITS_TARGET_REACHED         0x0400
#define STATUS_WORD_REGISTER_BITS_INTERNAL_LIMIT_ACTIVE  0x0800
#define STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_1      0x1000
#define STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_2      0x2000
#define STATUS_WORD_REGISTER_BITS_ANGLE_PROC_FINISHED    0x4000

//
// Control Word Register (CWR) bits meaning
//
//  --------------------------------------------------------------------------------
// | 15     9 |   8  |   7    | 6      4 |    3      |   2     |   1       |    0   |
// | -------------------------------------------------------------------------------
// | Reserved | Halt | Fault  | Mode     | Enable    | Quick   | Enable    | Switch |
// |          |      | reset  | specific | operation | stop    | voltage   | on     |
//  --------------------------------------------------------------------------------
//
#define CONTROL_WORD_REGISTER_BITS_SWITCH_ON             0x0001
#define CONTROL_WORD_REGISTER_BITS_ENABLE_VOLTAGE        0x0002
#define CONTROL_WORD_REGISTER_BITS_QUICK_STOP            0x0004
#define CONTROL_WORD_REGISTER_BITS_ENABLE_OPERATION      0x0008
#define CONTROL_WORD_REGISTER_BITS_MODE_SPECIFIC         0x0070
#define CONTROL_WORD_REGISTER_BITS_FAULT_RESET           0x0080
#define CONTROL_WORD_REGISTER_BITS_HALT                  0x0100
#define CONTROL_WORD_REGISTER_BITS_14_RESERVED           0x4000

// Useful object definitions (index, subindex)
#define OBJECT_CONTROL_WORD            0x6040, 0x00
#define OBJECT_STATUS_WORD             0x6041, 0x00

void Ingenia_SerialServoDrive::sendStateMachineCommand(const EStateMachineCommand eStateMachineCommand)
{
    bool isValidCommand = true;
    bool needsTransition = false;
    uint16_t u16TransitionAuxValue = 0;
    uint16_t u16ActualControlWord = 0;

    u16ActualControlWord = read(OBJECT_CONTROL_WORD);
    switch(eStateMachineCommand)
    {
        case COMMAND_SHUTDOWN:
            u16ActualControlWord &= ~(CONTROL_WORD_REGISTER_BITS_SWITCH_ON | CONTROL_WORD_REGISTER_BITS_FAULT_RESET);
            u16ActualControlWord |= (CONTROL_WORD_REGISTER_BITS_ENABLE_VOLTAGE | CONTROL_WORD_REGISTER_BITS_QUICK_STOP);
            break;

        case COMMAND_SWITCH_ON:
            u16ActualControlWord &= ~(CONTROL_WORD_REGISTER_BITS_ENABLE_OPERATION | CONTROL_WORD_REGISTER_BITS_FAULT_RESET);
            u16ActualControlWord |= (CONTROL_WORD_REGISTER_BITS_SWITCH_ON | CONTROL_WORD_REGISTER_BITS_ENABLE_VOLTAGE |
                                        CONTROL_WORD_REGISTER_BITS_QUICK_STOP);
            break;

        case COMMAND_SWITCH_ON_AND_ENABLE_OPERATION:
            u16ActualControlWord &= ~(CONTROL_WORD_REGISTER_BITS_FAULT_RESET);
            u16ActualControlWord |= (CONTROL_WORD_REGISTER_BITS_SWITCH_ON | CONTROL_WORD_REGISTER_BITS_ENABLE_VOLTAGE |
                                        CONTROL_WORD_REGISTER_BITS_QUICK_STOP | CONTROL_WORD_REGISTER_BITS_ENABLE_OPERATION);
            break;

        case COMMAND_DISABLE_VOLTAGE:
            u16ActualControlWord &= ~(CONTROL_WORD_REGISTER_BITS_ENABLE_VOLTAGE | CONTROL_WORD_REGISTER_BITS_FAULT_RESET);
            break;

        case COMMAND_QUICK_STOP:
            u16ActualControlWord &= ~(CONTROL_WORD_REGISTER_BITS_QUICK_STOP | CONTROL_WORD_REGISTER_BITS_FAULT_RESET);
            u16ActualControlWord |= CONTROL_WORD_REGISTER_BITS_ENABLE_VOLTAGE;
            break;

        case COMMAND_DISABLE_OPERATION:
            u16ActualControlWord &= ~(CONTROL_WORD_REGISTER_BITS_ENABLE_OPERATION | CONTROL_WORD_REGISTER_BITS_FAULT_RESET);
            u16ActualControlWord |= (CONTROL_WORD_REGISTER_BITS_SWITCH_ON | CONTROL_WORD_REGISTER_BITS_ENABLE_VOLTAGE |
                                        CONTROL_WORD_REGISTER_BITS_QUICK_STOP);
            break;

        case COMMAND_ENABLE_OPERATION:
            u16ActualControlWord &= ~(CONTROL_WORD_REGISTER_BITS_FAULT_RESET);
            u16ActualControlWord |= (CONTROL_WORD_REGISTER_BITS_SWITCH_ON | CONTROL_WORD_REGISTER_BITS_ENABLE_VOLTAGE |
                                        CONTROL_WORD_REGISTER_BITS_QUICK_STOP | CONTROL_WORD_REGISTER_BITS_ENABLE_OPERATION);
            break;

        case COMMAND_FAULT_RESET:
            if(u16ActualControlWord & CONTROL_WORD_REGISTER_BITS_FAULT_RESET)
            {
                u16ActualControlWord &= ~CONTROL_WORD_REGISTER_BITS_FAULT_RESET;
                needsTransition = true;
                u16TransitionAuxValue = CONTROL_WORD_REGISTER_BITS_FAULT_RESET;
            }
            else
            {
                u16ActualControlWord |= CONTROL_WORD_REGISTER_BITS_FAULT_RESET;
            }
            break;

        default:
            isValidCommand = false;
            break;
    }

    if (isValidCommand == true)
    {
        write(OBJECT_CONTROL_WORD, u16ActualControlWord);
        if (needsTransition == true)
        {
            u16ActualControlWord |= u16TransitionAuxValue;
            write(OBJECT_CONTROL_WORD, u16ActualControlWord);
        }
    }
}

void Ingenia_SerialServoDrive::setStateMachineStatus(const EStateMachineStatus eNewStateMachineStatus)
{
    EStateMachineStatus eCurrentStateMachineStatus;
    getStateMachineStatus(&eCurrentStateMachineStatus);
    goToStatusFrom(eCurrentStateMachineStatus, eNewStateMachineStatus);
}

void Ingenia_SerialServoDrive::getStateMachineStatus(EStateMachineStatus* peStateMachineStatus)
{
    uint16_t u16StatusWord;

    u16StatusWord = read(OBJECT_STATUS_WORD);
    *peStateMachineStatus = decodeStatusWord(u16StatusWord);
}

Ingenia_SerialServoDrive::EStateMachineStatus Ingenia_SerialServoDrive::decodeStatusWord(uint16_t u16StatusWord)
{
    uint16_t u16LSBMask;
    EStateMachineStatus eStateMachineStatus;

    u16LSBMask = STATUS_WORD_REGISTER_BITS_READY_TO_SWITCH_ON | STATUS_WORD_REGISTER_BITS_SWITCHED_ON | 
        STATUS_WORD_REGISTER_BITS_OPERATION_ENABLED | STATUS_WORD_REGISTER_BITS_FAULT;

    // ----------------------------------------------   //
    //      Value (binary)  |           Status          //
    // ----------------------------------------------   //
    // xxxx xxxx x0xx 0000  | Not ready to switch on    //
    // xxxx xxxx x1xx 0000  | Switched on disabled      //
    // xxxx xxxx x01x 0001  | Ready to switch on        //
    // xxxx xxxx x01x 0011  | Switched on               //
    // xxxx xxxx x01x 0111  | Operation enabled         //
    // xxxx xxxx x00x 0111  | Quick stop active         //
    // xxxx xxxx x0xx 1111  | Fault reaction active     //
    // xxxx xxxx x0xx 1000  | Fault                     //
    // ----------------------------------------------   //
    //

    _isInitialAngleDeterminationProcessFinished =
        ((u16StatusWord & STATUS_WORD_REGISTER_BITS_ANGLE_PROC_FINISHED) == STATUS_WORD_REGISTER_BITS_ANGLE_PROC_FINISHED);

    eStateMachineStatus = STATUS_UNKNOWN;
    switch((u16StatusWord & u16LSBMask))
    {
        case 0x00:
            if((u16StatusWord & STATUS_WORD_REGISTER_BITS_SWITCH_ON_DISABLED) == 0x00)
            {
                eStateMachineStatus = STATUS_NOT_READY_TO_SWITCH_ON;
            }
            else
            {
                eStateMachineStatus = STATUS_SWITCH_ON_DISABLED;
            }
            break;

        case 0x01:
            if((u16StatusWord & (STATUS_WORD_REGISTER_BITS_QUICK_STOP | STATUS_WORD_REGISTER_BITS_SWITCH_ON_DISABLED )) == 0x20)
            {
                eStateMachineStatus = STATUS_READY_TO_SWITCH_ON;
            }
            break;

        case 0x03:
            if((u16StatusWord & (STATUS_WORD_REGISTER_BITS_QUICK_STOP | STATUS_WORD_REGISTER_BITS_SWITCH_ON_DISABLED )) == 0x20)
            {
                eStateMachineStatus = STATUS_SWITCH_ON;
            }
            break;

        case 0x07:
            if((u16StatusWord & (STATUS_WORD_REGISTER_BITS_QUICK_STOP | STATUS_WORD_REGISTER_BITS_SWITCH_ON_DISABLED )) == 0x20)
            {
                eStateMachineStatus = STATUS_OPERATION_ENABLED;     
            }
            else if((u16StatusWord & (STATUS_WORD_REGISTER_BITS_QUICK_STOP | STATUS_WORD_REGISTER_BITS_SWITCH_ON_DISABLED )) == 0x00) 
            {
                eStateMachineStatus = STATUS_QUICK_STOP_ACTIVE;
            }
            break;
            
        case 0x0F:
            if((u16StatusWord & STATUS_WORD_REGISTER_BITS_SWITCH_ON_DISABLED) == 0x00)
            {
                eStateMachineStatus = STATUS_FAULT_REACTION_ACTIVE;
            }
            break;

        case 0x08:
            if((u16StatusWord & STATUS_WORD_REGISTER_BITS_SWITCH_ON_DISABLED) == 0x00)
            {
                eStateMachineStatus = STATUS_FAULT;
            }
            break;

        default:
            eStateMachineStatus = STATUS_UNKNOWN;
            break;
    }

    return eStateMachineStatus;
}

void Ingenia_SerialServoDrive::goToStatusFrom(const EStateMachineStatus eCurrentStateMachineStatus, const EStateMachineStatus eDestinationStateMachineStatus)
{
    EStateMachineStatus tCurrentStateMachineStatus = eCurrentStateMachineStatus;
    while (tCurrentStateMachineStatus != eDestinationStateMachineStatus)
    {
        EStateMachineStatus nextState = eDestinationStateMachineStatus;
        switch(tCurrentStateMachineStatus)
        {
            case STATUS_SWITCH_ON_DISABLED:
                if ((eDestinationStateMachineStatus == STATUS_READY_TO_SWITCH_ON) ||
                    (eDestinationStateMachineStatus == STATUS_SWITCH_ON) ||
                    (eDestinationStateMachineStatus == STATUS_OPERATION_ENABLED) ||
                    (eDestinationStateMachineStatus == STATUS_QUICK_STOP_ACTIVE))
                {
                    sendStateMachineCommand(COMMAND_SHUTDOWN);
                    nextState = STATUS_READY_TO_SWITCH_ON;
                }
                else
                {
                    return;
                }
                break;
            case STATUS_READY_TO_SWITCH_ON:
                if (eDestinationStateMachineStatus == STATUS_SWITCH_ON_DISABLED)
                {
                    sendStateMachineCommand(COMMAND_DISABLE_VOLTAGE);
                    nextState = STATUS_SWITCH_ON_DISABLED;
                }
                else if (eDestinationStateMachineStatus == STATUS_SWITCH_ON)
                {
                    sendStateMachineCommand(COMMAND_SWITCH_ON);
                    nextState = STATUS_SWITCH_ON;
                }
                else if ((eDestinationStateMachineStatus == STATUS_OPERATION_ENABLED) ||
                    (eDestinationStateMachineStatus == STATUS_QUICK_STOP_ACTIVE))
                {
                    sendStateMachineCommand(COMMAND_SWITCH_ON_AND_ENABLE_OPERATION);
                    nextState = STATUS_OPERATION_ENABLED;
                }
                else
                {
                    return;
                }
                break;
            case STATUS_SWITCH_ON:
                if (eDestinationStateMachineStatus == STATUS_SWITCH_ON_DISABLED)
                {
                    sendStateMachineCommand(COMMAND_DISABLE_VOLTAGE);
                    nextState = STATUS_SWITCH_ON_DISABLED;
                }
                else if (eDestinationStateMachineStatus == STATUS_READY_TO_SWITCH_ON)
                {
                    sendStateMachineCommand(COMMAND_SHUTDOWN);
                    nextState = STATUS_READY_TO_SWITCH_ON;
                }
                else if ((eDestinationStateMachineStatus == STATUS_OPERATION_ENABLED) ||
                    (eDestinationStateMachineStatus == STATUS_QUICK_STOP_ACTIVE))
                {
                    sendStateMachineCommand(COMMAND_ENABLE_OPERATION);
                    nextState = STATUS_OPERATION_ENABLED;
                }
                else
                {
                    return;
                }
                break;
            case STATUS_OPERATION_ENABLED:          
                if (eDestinationStateMachineStatus == STATUS_SWITCH_ON_DISABLED)
                {
                    sendStateMachineCommand(COMMAND_DISABLE_VOLTAGE);
                    nextState = STATUS_SWITCH_ON_DISABLED;
                }
                else if (eDestinationStateMachineStatus == STATUS_READY_TO_SWITCH_ON)
                {
                    sendStateMachineCommand(COMMAND_SHUTDOWN);
                    nextState = STATUS_READY_TO_SWITCH_ON;
                }
                else if (eDestinationStateMachineStatus == STATUS_SWITCH_ON)
                {
                    sendStateMachineCommand(COMMAND_DISABLE_OPERATION);
                    nextState = STATUS_SWITCH_ON;
                }
                else if (eDestinationStateMachineStatus == STATUS_QUICK_STOP_ACTIVE)
                {
                    sendStateMachineCommand(COMMAND_QUICK_STOP);
                    nextState = STATUS_QUICK_STOP_ACTIVE;
                }
                else
                {
                    return;
                }
                break;
            case STATUS_QUICK_STOP_ACTIVE:
                if ((eDestinationStateMachineStatus == STATUS_SWITCH_ON_DISABLED) ||
                    (eDestinationStateMachineStatus == STATUS_READY_TO_SWITCH_ON) ||
                    (eDestinationStateMachineStatus == STATUS_SWITCH_ON) ||
                    (eDestinationStateMachineStatus == STATUS_OPERATION_ENABLED))
                {
                    sendStateMachineCommand(COMMAND_DISABLE_VOLTAGE);
                    nextState = STATUS_SWITCH_ON_DISABLED;
                }
                else
                {
                    return;
                }
                break;
            case STATUS_FAULT:
                if ((eDestinationStateMachineStatus == STATUS_SWITCH_ON_DISABLED) ||
                    (eDestinationStateMachineStatus == STATUS_READY_TO_SWITCH_ON) ||
                    (eDestinationStateMachineStatus == STATUS_SWITCH_ON) ||
                    (eDestinationStateMachineStatus == STATUS_OPERATION_ENABLED) ||
                    (eDestinationStateMachineStatus == STATUS_QUICK_STOP_ACTIVE))
                {
                    sendStateMachineCommand(COMMAND_FAULT_RESET);
                    nextState = STATUS_SWITCH_ON_DISABLED;
                }
                else
                {
                    return;
                }
                break;
            default:
                {
                    return;
                }
                break;
        }

        verifyStatusIsReached(nextState);
        tCurrentStateMachineStatus = nextState;
    }
}

bool Ingenia_SerialServoDrive::verifyStatusIsReached(const EStateMachineStatus eNextStateMachineStatus)
{
    EStateMachineStatus eActualStateMachineStatus;
    getStateMachineStatus(&eActualStateMachineStatus);

    unsigned long long started_waiting_at = millis();
    while (eNextStateMachineStatus != eActualStateMachineStatus)
    {
        // Check if timeout excedeed
        if (millis() - started_waiting_at > 1000 )
            return false;
        getStateMachineStatus(&eActualStateMachineStatus);
    }
    return true;
}

void Ingenia_SerialServoDrive::enableMotor()
{
    setStateMachineStatus(STATUS_OPERATION_ENABLED);
}

void Ingenia_SerialServoDrive::disableMotor()
{
    // this->StopMacros();
    // this->DisableInterrupts();
    EStateMachineStatus currentStateMachineStatus;
    getStateMachineStatus(&currentStateMachineStatus);

    if (currentStateMachineStatus == STATUS_OPERATION_ENABLED)
    {
        setStateMachineStatus(STATUS_SWITCH_ON);
    }
}

#define TARGET_POSITION_INDEX       0x607A
#define TARGET_POSITION_SUBINDEX    0x0
#define VELOCITY_INDEX              0x60FF
#define VELOCITY_SUBINDEX           0x0
#define TORQUE_INDEX                0x6071
#define TORQUE_SUBINDEX             0x0

void Ingenia_SerialServoDrive::setTargetVelocity(int32_t value)
{
    write(VELOCITY_INDEX, VELOCITY_SUBINDEX, value);
}

void Ingenia_SerialServoDrive::setTargetTorque(int16_t value)
{
    write(TORQUE_INDEX, TORQUE_SUBINDEX, value);
}


#define MODES_OF_OPERATION_INDEX              0x6060
#define MODES_OF_OPERATION_SUBINDEX           0x00

void Ingenia_SerialServoDrive::setModeOfOperation(EDriveModes driverMode)
{
    write(MODES_OF_OPERATION_INDEX, MODES_OF_OPERATION_SUBINDEX, (uint8_t) driverMode);
}

#define MODE_SPECIFIC_BITS_NEW_SETPOINT   0x0010
#define MODE_SPECIFIC_BITS_CHANGE_SET 0x0020
#define MODE_SPECIFIC_BITS_ABS_REL    0x0040

void Ingenia_SerialServoDrive::setTargetPosition(int32_t value, bool isImmediate, bool isRelative, bool isHaltEnabled)
{
    write(TARGET_POSITION_INDEX, TARGET_POSITION_SUBINDEX, value);

    uint16_t u16ActualControlWord = 0;
    u16ActualControlWord = read(OBJECT_CONTROL_WORD);

    if ((u16ActualControlWord & MODE_SPECIFIC_BITS_NEW_SETPOINT) > 0)
    { // Low new setpoint flag if needed
        u16ActualControlWord &= ~(MODE_SPECIFIC_BITS_NEW_SETPOINT);
        write(OBJECT_CONTROL_WORD, u16ActualControlWord);
    }

    // Raise New Setpoint Bit
    u16ActualControlWord |= 1 << 4;

    u16ActualControlWord &= ~(MODE_SPECIFIC_BITS_CHANGE_SET);
    u16ActualControlWord |= isImmediate << 5;

    u16ActualControlWord &= ~(MODE_SPECIFIC_BITS_ABS_REL);
    u16ActualControlWord |= isRelative << 6;

    u16ActualControlWord &= ~(CONTROL_WORD_REGISTER_BITS_HALT);
    u16ActualControlWord |= isHaltEnabled << 8;

    write(OBJECT_CONTROL_WORD, u16ActualControlWord);

    //Low New Setpoint
    u16ActualControlWord &= ~(MODE_SPECIFIC_BITS_NEW_SETPOINT);
    write(OBJECT_CONTROL_WORD, u16ActualControlWord);
}


#define POSITION_ACTUAL_INDEX         0x6064
#define POSITION_ACTUAL_SUBINDEX      0x0
#define TORQUE_ACTUAL_INDEX           0x6077
#define TORQUE_ACTUAL_SUBINDEX        0x0
#define VELOCITY_ACTUAL_INDEX         0x606C
#define VELOCITY_ACTUAL_SUBINDEX      0x0


void Ingenia_SerialServoDrive::setTargetPosition(int32_t value)
{
    setTargetPosition(value, true, false, false);
}

int32_t Ingenia_SerialServoDrive::getActualPosition()
{
    return read(POSITION_ACTUAL_INDEX, POSITION_ACTUAL_SUBINDEX);
}

int32_t Ingenia_SerialServoDrive::getActualVelocity()
{
    return read(VELOCITY_ACTUAL_INDEX, VELOCITY_ACTUAL_SUBINDEX);
}

int16_t Ingenia_SerialServoDrive::getActualTorque()
{
    return read(TORQUE_ACTUAL_INDEX, TORQUE_ACTUAL_SUBINDEX);
}

void Ingenia_SerialServoDrive::enableBinary()
{
    this->write(0x0, 0x2000, 0x8, 1, (uint8_t) 1);
    _isBinaryEnabled = true;
}

void Ingenia_SerialServoDrive::disableBinary()
{
    this->write(0x0, 0x2000, 0x8, 1, (uint8_t) 0);
    _isBinaryEnabled = false;
}


#define STATUSWORD_INDEX         0x6041
#define STATUSWORD_SUBINDEX      0x0

uint16_t Ingenia_SerialServoDrive::getStatusword()
{
    return read(STATUSWORD_INDEX, STATUSWORD_SUBINDEX);
}

bool Ingenia_SerialServoDrive::statuswordIsReadyToSwitchOn()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_READY_TO_SWITCH_ON) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsSwitchedOn()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_SWITCHED_ON) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsOperationEnabled()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_OPERATION_ENABLED) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsFault()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_FAULT) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsVoltageEnabled()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_VOLTAGE_ENABLED) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsQuickStop()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_QUICK_STOP) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsSwitchOnDisabled()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_SWITCH_ON_DISABLED) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsWarning()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_WARNING) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsRemote()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_REMOTE) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsTargetReached()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_TARGET_REACHED) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsInternalLimitActive()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_INTERNAL_LIMIT_ACTIVE) > 0;
}

bool Ingenia_SerialServoDrive::statuswordIsInitialAngleDeterminationProcessFinished()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_ANGLE_PROC_FINISHED) > 0;
}

bool Ingenia_SerialServoDrive::homingStatusIsInProgress()
{
    return ((getStatusword() & STATUS_WORD_REGISTER_BITS_TARGET_REACHED) == 0) &&
           ((getStatusword() & STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_1) == 0) &&
           ((getStatusword() & STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_2) == 0);
}

bool Ingenia_SerialServoDrive::homingStatusIsError()
{
    return (getStatusword() & STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_2) > 0;
}

bool Ingenia_SerialServoDrive::homingStatusIsSuccess()
{
    return ((getStatusword() & STATUS_WORD_REGISTER_BITS_TARGET_REACHED) > 0) &&
           ((getStatusword() & STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_1) > 0) &&
           ((getStatusword() & STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_2) == 0);
}

bool Ingenia_SerialServoDrive::homingStatusIsAttained()
{
    return ((getStatusword() & STATUS_WORD_REGISTER_BITS_TARGET_REACHED) == 0) &&
           ((getStatusword() & STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_1) > 0) &&
           ((getStatusword() & STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_2) == 0);
}


bool Ingenia_SerialServoDrive::homingStatusIsInterrupted()
{
    return ((getStatusword() & STATUS_WORD_REGISTER_BITS_TARGET_REACHED) > 0) &&
           ((getStatusword() & STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_1) == 0) &&
           ((getStatusword() & STATUS_WORD_REGISTER_BITS_OPMODE_SPECIFIC_2) == 0);
}

bool Ingenia_SerialServoDrive::homingStatusNotStarted()
{
    return this->homingStatusIsInterrupted();
}

void Ingenia_SerialServoDrive::doHoming(int8_t i8HomingMethod)
{
    this->setModeOfOperation(DRIVE_MODE_HOMING);
    this->write(0x6098, 0x0, i8HomingMethod);

    this->enableMotor();

    uint16_t controlWord;
    controlWord = this->read(OBJECT_CONTROL_WORD);
    controlWord &= ~ 0x0010;
    this->write(OBJECT_CONTROL_WORD, controlWord);
    controlWord = this->read(OBJECT_CONTROL_WORD);
    controlWord |= 0x0010;
    this->write(OBJECT_CONTROL_WORD, controlWord);
}
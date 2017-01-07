#include "Dynamixel_Servo.h"
#include <Arduino.h>

/*--------------------------------------------------------------------------------------------*/
#define HALF_DUPLEX_DIRECTION_OUTPUT LOW
#define HALF_DUPLEX_DIRECTION_INPUT  HIGH
HardwareSerial* servo_serial = NULL;
int servo_half_duplex_direction_pin;

/*--------------------------------------------------------------------------------------------*/
typedef enum servo_instruction_enum
{
  SERVO_INSTRUCTION_PING         = 0x01,
  SERVO_INSTRUCTION_READ_DATA    = 0x02,
  SERVO_INSTRUCTION_WRITE_DATA   = 0x03,
  SERVO_INSTRUCTION_REG_WRITE    = 0x04,
  SERVO_INSTRUCTION_ACTION       = 0x05,
  SERVO_INSTRUCTION_RESET        = 0x06,
  SERVO_INSTRUCTION_SYNC_WRITE   = 0x83,
}servo_instruction_t;

/*--------------------------------------------------------------------------------------------*/
servo_error_t _servo_set            (uint8_t id, servo_register_t reg, float  val, int timeout_ms, bool do_now);
servo_error_t _servo_set_raw_byte   (uint8_t id, servo_register_t reg, uint8_t  value, int timeout_ms, bool do_now);
servo_error_t _servo_set_raw_word   (uint8_t id, servo_register_t reg, uint16_t value, int timeout_ms,   bool do_now);
servo_error_t _servo_set_raw_page   (uint8_t id, servo_register_t reg, uint8_t  values[], int num_bytes, int timeout_ms, bool do_now);
servo_error_t servo_get_response    (uint8_t id, uint8_t result[], int result_size, int timeout_ms);
servo_error_t servo_send_instruction(uint8_t id, servo_instruction_t instruction, 
                                     uint8_t parameters[], uint8_t num_parameters, 
                                     uint8_t result[], int num_results, int timeout_ms);

/*--------------------------------------------------------------------------------------------*/ 
void servo_init(HardwareSerial* serial_port, int half_duplex_direction_pin, uint32_t baud)
{
  servo_serial = serial_port;
  servo_half_duplex_direction_pin = half_duplex_direction_pin;
  servo_serial->begin(baud);
  
  pinMode(half_duplex_direction_pin, OUTPUT);
  digitalWrite(half_duplex_direction_pin, HALF_DUPLEX_DIRECTION_INPUT);
}

/*--------------------------------------------------------------------------------------------*/
bool servo_register_is_word(servo_register_t reg)
{
  bool result = false;
  
  switch(reg)
    {
      case SERVO_REGISTER_MODEL_NUMBER        :
      case SERVO_REGISTER_MAX_ANGLE           :
      case SERVO_REGISTER_MIN_ANGLE           :
      case SERVO_REGISTER_MAX_TORQUE          :
      case SERVO_REGISTER_GOAL_ANGLE          :
      case SERVO_REGISTER_MOVING_SPEED        :
      case SERVO_REGISTER_TORQUE_LIMIT        :
      case SERVO_REGISTER_PRESENT_ANGLE       :
      case SERVO_REGISTER_PRESENT_SPEED       :
      case SERVO_REGISTER_PRESENT_TORQUE      :
      case SERVO_REGISTER_PUNCH               :
      case SERVO_REGISTER_CURRENT_CONSUMPTION :
      case SERVO_REGISTER_GOAL_TORQUE         :
        result = true;
        break;
      default: break;
    }

  return result;
}
                                     
/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_factory_reset    (uint8_t id, int timeout_ms)
{
  return servo_send_instruction(id, SERVO_INSTRUCTION_RESET, NULL, 0, NULL, 0, timeout_ms);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_ping             (uint8_t id, int timeout_ms)
{
  return servo_send_instruction(id, SERVO_INSTRUCTION_PING, NULL, 0, NULL, 0, timeout_ms);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_get              (uint8_t id, servo_register_t reg, float* result, int timeout_ms)
{
  servo_error_t error;
  bool is_word = servo_register_is_word(reg);
  
  if(is_word)
    {
      uint16_t raw;
      error = servo_get_raw_word(id, reg, &raw, timeout_ms);
      *result = servo_raw_to_anything(reg, (int)raw);
    }
  else
    {
      uint8_t raw;
      error = servo_get_raw_byte(id, reg, &raw, timeout_ms);
      *result = servo_raw_to_anything(reg, (int)raw);  
    }
    
  return error;
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t _servo_set              (uint8_t id, servo_register_t reg, float  val, int timeout_ms, bool do_now)
{
  servo_error_t error = SERVO_ERROR_RANGE;
  bool is_word = servo_register_is_word(reg);
  int  raw = servo_anything_to_raw(reg, val);
  
  if(is_word)
    {
      if((raw >= 0) && (raw <= 0xFFFF))
        error = _servo_set_raw_word(id, reg, (uint16_t)raw, timeout_ms, do_now);
    }
  else
    {
      if((raw >= 0) && (raw <= 0xFF))
        error = _servo_set_raw_byte(id, reg, (uint8_t)raw, timeout_ms, do_now);
    }

  return error;
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_set              (uint8_t id, servo_register_t reg, float  val, int timeout_ms)
{  
  return _servo_set(id, reg, val, timeout_ms, true);  
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_set_multiple     (uint8_t ids[], servo_register_t start_reg, float values[], int num_ids, int num_values_per_servo)
{  
  int bytes_per_value[num_values_per_servo];
  int* b = bytes_per_value;
  int bytes_per_servo = 0;
  int i, j, k;
  int addr = (int)start_reg;
  
  uint8_t  raw_bytes[num_ids * 2]; //worst case scenario
  uint8_t* r = raw_bytes;
  int      raw_anything;
  
  for(i=0; i<num_values_per_servo; i++)
    {
      *b = (servo_register_is_word((servo_register_t)addr)) ? 2 : 1;
      bytes_per_servo += *b;
      addr += *b++;
    }
  
  for(i=0; i<num_ids; i++)
    {
      addr = start_reg;
      b = bytes_per_value;
      for(j=0; j<num_values_per_servo; j++)
        {
          raw_anything = servo_anything_to_raw((servo_register_t)addr,  *values++);

          for(k=0; k<*b; k++)
            {
              *r++ = (raw_anything & 0xFF);
              raw_anything >>= 8;
            }
          addr += *b++;
        }
    }
  
  return servo_set_multiple_raw(ids, start_reg, raw_bytes, num_ids, bytes_per_servo);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_prepare          (uint8_t id, servo_register_t reg, float  val, int timeout_ms)
{  
  return _servo_set(id, reg, val, timeout_ms, false);  
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_do_prepared      (uint8_t id, int timeout_ms)
{
  return servo_send_instruction(id, SERVO_INSTRUCTION_ACTION, NULL, 0, NULL, 0, timeout_ms);
}

/*--------------------------------------------------------------------------------------------*/
char* servo_errors_to_string  (servo_error_t error)
{
  static const char* const_error_strings[] /*PROGMEM*/ = 
    {
      "voltage",
      "angle",
      "overheat",
      "range",
      "checksum",
      "overload",
      "instruction",
      "unknown",
      "timeout",
      "invalid_response",
    };
    
  static char result[100];
  char* r = result;
  
  if(error != SERVO_NO_ERROR)
    {
      int i;
      for(i=0; i<10; i++)
        if ((1 << i) & error)
          {
            //this saves over 1K of code space
            //r += sprintf(r, "%s ", const_error_strings[i]);
            const char* s = const_error_strings[i];
            while(*s != '\0')
              *r++ = *s++;
            *r++ = ' ';
          }
    }
    
  *r = '\0';
  return result;
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_get_raw_byte     (uint8_t id, servo_register_t reg, uint8_t*  result, int timeout_ms)
{
  uint8_t params[2] = {reg, 1};
  return servo_send_instruction(id, SERVO_INSTRUCTION_READ_DATA, params, 2, result, 1, timeout_ms);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_get_raw_word     (uint8_t id, servo_register_t reg, uint16_t* result, int timeout_ms)
{
  uint8_t params [2] = {reg, 2};
  uint8_t results[2] = {0, 0};
  servo_error_t error;

  error = servo_send_instruction(id, SERVO_INSTRUCTION_READ_DATA, params, 2, results, 2, timeout_ms);
  *result = ((uint16_t)results[1] << 8) | ((uint16_t)results[0]);
  
  return error;
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_get_raw_page     (uint8_t id, servo_register_t reg, uint8_t   result[], int num_bytes, int timeout_ms)
{
  uint8_t params [2] = {reg, num_bytes};
  return servo_send_instruction(id, SERVO_INSTRUCTION_READ_DATA, params, 2, result, num_bytes, timeout_ms);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t _servo_set_raw_byte     (uint8_t id, servo_register_t reg, uint8_t  value, int timeout_ms,  bool do_now)
{
  servo_instruction_t instruction = (do_now) ? SERVO_INSTRUCTION_WRITE_DATA : SERVO_INSTRUCTION_REG_WRITE;
  uint8_t params[2] = {reg, value};
  return servo_send_instruction(id, instruction, params, 2, NULL, 0, timeout_ms);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t _servo_set_raw_word     (uint8_t id, servo_register_t reg, uint16_t value, int timeout_ms,  bool do_now)
{
  servo_instruction_t instruction = (do_now) ? SERVO_INSTRUCTION_WRITE_DATA : SERVO_INSTRUCTION_REG_WRITE;
  uint8_t params[3] = {reg, (value & 0xFF), (value >> 8)};
  return servo_send_instruction(id, instruction, params, 3, NULL, 0, timeout_ms);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t _servo_set_raw_page     (uint8_t id, servo_register_t reg, uint8_t  values[], int num_bytes, int timeout_ms, bool do_now)
{
  servo_instruction_t instruction = (do_now) ? SERVO_INSTRUCTION_WRITE_DATA : SERVO_INSTRUCTION_REG_WRITE;
  uint8_t params[num_bytes + 1];
  uint8_t *p;
  int n = num_bytes;
  *p++ = reg;
  while(n-- < 0)
    *p++ = *values++;
  
  return servo_send_instruction(id, instruction, params, num_bytes, NULL, 0, timeout_ms);  
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_set_raw_byte     (uint8_t id, servo_register_t reg, uint8_t   value,    int timeout_ms)
{
  return _servo_set_raw_byte(id, reg, value, timeout_ms, true);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_set_raw_word     (uint8_t id, servo_register_t reg, uint16_t  value,    int timeout_ms)
{
  return _servo_set_raw_word(id, reg, value, timeout_ms, true);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_set_raw_page     (uint8_t id, servo_register_t reg, uint8_t   values[], int num_bytes, int timeout_ms)
{
  return _servo_set_raw_page(id, reg, values, num_bytes, timeout_ms, true);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_prepare_raw_byte (uint8_t id, servo_register_t reg, uint8_t   value,    int timeout_ms)
{
  return _servo_set_raw_byte(id, reg, value, timeout_ms, false);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_prepare_raw_word (uint8_t id, servo_register_t reg, uint16_t  value,    int timeout_ms)
{
  return _servo_set_raw_word(id, reg, value, timeout_ms, false);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_prepare_raw_page (uint8_t id, servo_register_t reg, uint8_t   values[], int num_bytes, int timeout_ms)
{
  return _servo_set_raw_page(id, reg, values, num_bytes, timeout_ms, false);
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_set_multiple_raw(uint8_t ids[], servo_register_t start_reg, uint8_t bytes[], int num_ids, int bytes_per_servo)
{
  int i, j;
  uint8_t num_params = num_ids * (bytes_per_servo + 1) + 2;
  uint8_t params[num_params];
  uint8_t *p = params;
  
  *p++ = start_reg;
  *p++ = bytes_per_servo;
  
  for(i=0; i<num_ids; i++)
    {   
      *p++ = *ids++;
      for(j=0; j<bytes_per_servo; j++)
        *p++ = *bytes++;
    }    
    
  return servo_send_instruction(SERVO_BROADCAST_ID, SERVO_INSTRUCTION_SYNC_WRITE, params, num_params, NULL, 0, 0); 
}

/*--------------------------------------------------------------------------------------------*/
void* servo_get_conversion_function_for_register(servo_register_t reg, bool to_raw)
{
  int (*result)() = NULL;
  
  switch(reg)
    {
      case SERVO_REGISTER_BAUD_RATE          :
        result = (to_raw) ? 
          (int (*)()) servo_baud_bps_to_raw:
          (int (*)()) servo_raw_to_baud_bps; 
        break;
        
      case SERVO_REGISTER_RETURN_DELAY_TIME  :
        result = (to_raw) ? 
          (int (*)()) servo_return_delay_usec_to_raw:
          (int (*)()) servo_raw_to_return_delay_usec;
        break;
        
      case SERVO_REGISTER_MAX_ANGLE          :
      case SERVO_REGISTER_MIN_ANGLE          :
      case SERVO_REGISTER_GOAL_ANGLE         :
      case SERVO_REGISTER_PRESENT_ANGLE      :
        result = (to_raw) ?
          (int (*)()) servo_radians_to_raw:
          (int (*)()) servo_raw_to_radians;
        break;        
                  
      case SERVO_REGISTER_MIN_VOLTAGE        :  
      case SERVO_REGISTER_MAX_VOLTAGE        :
      case SERVO_REGISTER_PRESENT_VOLTAGE    :
        result = (to_raw) ?
          (int (*)()) servo_volts_to_raw: 
          (int (*)()) servo_raw_to_volts;
        break;
        
      case SERVO_REGISTER_MAX_TORQUE         :
      case SERVO_REGISTER_PRESENT_TORQUE     :
      case SERVO_REGISTER_TORQUE_LIMIT       :
      case SERVO_REGISTER_GOAL_TORQUE        :
        result = (to_raw) ?
          (int (*)()) servo_torque_percentage_to_raw:
          (int (*)()) servo_raw_to_torque_percentage; 
        break;
                     
      case SERVO_REGISTER_D_GAIN             :
        result = (to_raw) ?
          (int (*)()) servo_d_gain_to_raw:
          (int (*)()) servo_raw_to_d_gain;
        break;
        
      case SERVO_REGISTER_I_GAIN             :
        result = (to_raw) ?
          (int (*)()) servo_i_gain_to_raw:
          (int (*)()) servo_raw_to_i_gain;
        break;
        
      case SERVO_REGISTER_P_GAIN             :
        result = (to_raw) ?
          (int (*)()) servo_p_gain_to_raw:
          (int (*)()) servo_raw_to_p_gain;
        break;
        
      case SERVO_REGISTER_MOVING_SPEED       :        
      case SERVO_REGISTER_PRESENT_SPEED      :
        result = (to_raw) ?
          (int (*)()) servo_radians_per_sec_to_raw:
          (int (*)()) servo_raw_to_radians_per_sec;
        break;
        
      case SERVO_REGISTER_CURRENT_CONSUMPTION:
        result = (to_raw) ?
          (int (*)()) servo_amperes_to_raw:
          (int (*)()) servo_raw_to_amperes; 
        break;
        
      case SERVO_REGISTER_GOAL_ACCELERATION  :
        result = (to_raw) ?
          (int (*)()) servo_radians_per_sec_2_to_raw:
          (int (*)()) servo_raw_to_radians_per_sec_2;
        break;
      
      default:
        result = NULL;
      break;
    }

  return (void*)result;
}

/*--------------------------------------------------------------------------------------------*/
int      servo_anything_to_raw (servo_register_t reg, float anything)
{
  int (*conversion)(float) = (int (*)(float))servo_get_conversion_function_for_register(reg, true);
  return (conversion == NULL) ? (int)(anything + 0.5) : conversion(anything);
}

/*--------------------------------------------------------------------------------------------*/
float    servo_raw_to_anything (servo_register_t reg, int raw)
{
  float (*conversion)(int) = (float (*)(int))servo_get_conversion_function_for_register(reg, false);
  return (conversion == NULL) ? (float) raw : conversion(raw);
}

/*--------------------------------------------------------------------------------------------*/
int  servo_baud_bps_to_raw(float baud)
{
  uint8_t result = 0xFF;
  float actual_baud;
  
  if((baud <= 2000000) && (baud >= 8000))
    result = (int)(((2000000.0 / (float)baud) - 1) + 0.5);
   
  else switch((unsigned) baud)
    {
      case 2250000: result = 250; break;
      case 2500000: result = 251; break;
      case 3000000: result = 252; break;
      default: break;
    }
  
  actual_baud = servo_raw_to_baud_bps(result);
  if(result * 0.03 > fabs(result - actual_baud))
    result = 0xFF; //invalid baud value, servo will return error and baud will not change;
  
  return result;
}

/*--------------------------------------------------------------------------------------------*/
float    servo_raw_to_baud_bps(int raw )
{
  uint32_t result = 0;
  
  if(raw <= 249)
    result = 2000000 / (raw + 1);
  else switch(raw)
    {
      case 250: result = 2250000; break;
      case 251: result = 2500000; break;
      case 252: result = 3000000; break;
      default: break;
    }
  
  return result;
}

/*as functions and not macros so pointers can be used */
/*--------------------------------------------------------------------------------------------*/
int   servo_return_delay_usec_to_raw (float usec)   {return usec * 0.5;}
/*--------------------------------------------------------------------------------------------*/
float servo_raw_to_return_delay_usec (int raw)      {return raw * 2  ;}
/*--------------------------------------------------------------------------------------------*/
int   servo_radians_to_raw           (float rad)    
{
  float two_pi = 2 * M_PI;
  while(rad >= two_pi) rad -= two_pi;
  while(rad <  0     ) rad += two_pi;
  
  return  (int) (rad * (4095.0 / two_pi) + 0.5);
}
/*--------------------------------------------------------------------------------------------*/
float servo_raw_to_radians           (int raw)      {return raw * (6.28318531 / 4095.0);}
/*--------------------------------------------------------------------------------------------*/
int   servo_torque_percentage_to_raw (float percent)
{
  int result;
  int is_counter_clockwise = (percent < 0);
  if(is_counter_clockwise) percent *= -1;
  result = percent * 10.23 + 0.5;
  if(is_counter_clockwise) result |= 1024;
  
  return result;
}
/*--------------------------------------------------------------------------------------------*/
float servo_raw_to_torque_percentage (int raw)      
{
  float result;
  int is_negative = (raw >= 0x400); //B10000000000
  raw &= 0x3FF;                     //B01111111111
  result = raw * (100.0 / 1023.0);
  if(is_negative) result *= -1;
  
  return result;
}
/*--------------------------------------------------------------------------------------------*/
int   servo_p_gain_to_raw            (float gain)   {return gain * 0.8 + 0.5;}
/*--------------------------------------------------------------------------------------------*/
float servo_raw_to_p_gain            (int raw)      {return raw * 1.25;}
/*--------------------------------------------------------------------------------------------*/
int   servo_i_gain_to_raw            (float gain)   {return gain * (2048.0 / 1000.0) + 0.5;}
/*--------------------------------------------------------------------------------------------*/
float servo_raw_to_i_gain            (int raw)      {return raw * (1000.0 / 2048.0);}
/*--------------------------------------------------------------------------------------------*/
int   servo_d_gain_to_raw            (float gain)   {return gain * 250;}
/*--------------------------------------------------------------------------------------------*/
float servo_raw_to_d_gain            (int raw)      {return raw * 0.004;}
/*--------------------------------------------------------------------------------------------*/
int   servo_volts_to_raw             (float volts)  {return volts * 10;}
/*--------------------------------------------------------------------------------------------*/
float servo_raw_to_volts             (int raw)      {return raw * 0.1;}
/*--------------------------------------------------------------------------------------------*/
int   servo_amperes_to_raw           (float amps)   {return amps * 222.22222 + 2048.5;}
/*--------------------------------------------------------------------------------------------*/
float servo_raw_to_amperes           (int raw)      {return 0.0045 * (raw - 2048);}
/*--------------------------------------------------------------------------------------------*/
int   servo_radians_per_sec_to_raw   (float rad)
{
  int is_counter_clockwise = (rad < 0);
  if(is_counter_clockwise) rad *= -1; 
  int result = (rad * (60 / (0.114 * 6.28318531))) + 0.5;
  if(result > 1023) result = 1023;
  
  //wheel mode needs this
  //if(result == 0) result = 1024;
  
  //joint mode needs this
  if(result == 0) result = 1;
  
  if(is_counter_clockwise) result |= 0x400;
  
  return result;
  
  //for joint mode 0 should indicate the motor's max speed,
  //but this is not implemented because 1023 is twice as fast as
  // the motor's max speed anyhow;
  //return (result < 1024) ? result : 0;
}

/*--------------------------------------------------------------------------------------------*/
float servo_raw_to_radians_per_sec   (int raw)
{
  //return (raw > 0) ? (raw - 1024) * (SERVO_MAX_SPEED / 1024.0) : SERVO_MAX_SPEED;
  
  float SERVO_MAX_SPEED = 13;
  float result;
  int is_negative = raw >= 0x400; //B10000000000
  raw &= 0x3FF;                   //B01111111111
  result = raw * (0.114 * 6.28318531 / 60.0); // rad / sec
  if(is_negative) result *= -1;
  
  return (raw > 0) ? result : SERVO_MAX_SPEED;
}

/*--------------------------------------------------------------------------------------------*/
int   servo_radians_per_sec_2_to_raw   (float rad)
{ 
  int result = rad * (1.0 / 0.14979598) + 0.5;
  return (result < 255) ? result : 0;
}

/*--------------------------------------------------------------------------------------------*/
float servo_raw_to_radians_per_sec_2   (int raw)
{
  float SERVO_MAX_ACCEL = 39;
  return (raw > 0) ? raw * 0.14979598 : SERVO_MAX_ACCEL;
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_send_instruction(uint8_t id, servo_instruction_t instruction, 
                                     uint8_t parameters[], uint8_t num_parameters, 
                                     uint8_t result[], int num_results, int timeout_ms)
{
  int data_size = num_parameters + 6;
  uint8_t data[data_size];
  uint8_t *d = data;
  uint8_t checksum;
  servo_error_t error = SERVO_NO_ERROR;
  
  //could check instruction and number of parameters here;

  *d++ = 0xFF;
  *d++ = 0xFF;  
  *d++ = id;
  *d++ = num_parameters + 2;
  *d++ = (uint8_t) instruction;

  checksum = data[2] + data[3] + data[4];
  
  while(num_parameters-- > 0)
    {
      checksum += *parameters;
      *d++ = *parameters++;
    }

  *d++ = ~checksum;
  
  digitalWrite(servo_half_duplex_direction_pin, HALF_DUPLEX_DIRECTION_OUTPUT);
  //Delay was necessary when on solderless breadboard, but not on custom PCB (probably stray capacitance)
  //delayMicroseconds(10);
  
  while(servo_serial->available()) servo_serial->read(); //remove junk from buffer (there shouldn't be any)
  servo_serial->write(data, data_size);
  
  //Datasheet says to disable and enable interrupts here but the given reason seems
  //unnecessary, and without interrupts, flush won't know that the last byte has been transmitted
  //servo_serial->write(data, data_size-1);
  //cli();
  //servo_serial->write(data[data_size - 1]);
  servo_serial->flush(); //this waits for all data to be transmitted (does not flush input buffer)
  digitalWrite(servo_half_duplex_direction_pin, HALF_DUPLEX_DIRECTION_INPUT);
  //sei();
  
  if(id != SERVO_BROADCAST_ID)
    error = servo_get_response(id, result, num_results, timeout_ms);

  return error;
}

/*--------------------------------------------------------------------------------------------*/
servo_error_t servo_get_response    (uint8_t id, uint8_t result[], int result_size, int timeout_ms)
{
  int data_size = result_size + 6;
  uint8_t data[data_size];
  uint8_t* d = data;
  servo_error_t error = SERVO_NO_ERROR;
  uint8_t checksum;
  timeout_ms *= 100;

  while ((d - data) < data_size)
    {
      if(servo_serial->available())
        *d++ = servo_serial->read();
      else if(timeout_ms-- > 0)
        delayMicroseconds(10);
      else
        break;
    }
  
  if((d-data) != data_size) 
    return SERVO_ERROR_TIMEOUT;

  if((data[0] != 0xFF)          || 
     (data[1] != 0xFF)          ||
     (data[2] != id  )          ||
     (data[3] != data_size - 4)  )
    return SERVO_ERROR_INVALID_RESPONSE;
    
  error = (servo_error_t)data[4];
  
  checksum = data[2] + data[3] + data[4];
  d = data + 5;
  while(result_size-- > 0)
    {
      checksum += *d;
      *result++ = *d++;
    }
  checksum = ~checksum;
  if(*d != checksum)
    error = (servo_error_t) (error | SERVO_ERROR_INVALID_RESPONSE);
    
  return error;
}


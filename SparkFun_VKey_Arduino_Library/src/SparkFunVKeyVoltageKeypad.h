/******************************************************************************
 VKey.h
 VKey Voltage Keypad library
 By Byron Jacquot @ SparkFun Electronics
 February 4, 2014
 https://github.com/sparkfun/VKey_Voltage_Keypad

 This library interfaces with the VKey analog keypad.  It relies on the 
 Arduino analogRead functionality.
 The keypad has 12 keys, and outputs a unique Voltage for each key.  It allows
 an application to interface with up to 12 keys using only one ADC line.

 To use the library, instantiate a VKey object, then call checkKeys() to
 read the keypad.

 See SparkFun_VKey_demo_main.ino for a usage example.
 
 Also, see the contents of the /documents/ directory for information about 
 scaling and calibration.  They'll be useful if you're porting the library to 
 other supply or reference voltages, or ADC bit widths.
 
 Resources:
 no additional library requirements.

 Development environment specifics:
 Developed on Arduino IDE 1.0.5
 Tested on a 5V Redboard and 3.3V Pro Mini
 
  **UPDATED FOR ARDUINO 1.6.4 5/2015**

 This code is beerware; if you see me (or any other SparkFun employee) at the
 local, and you've found our code helpful, please buy us a round!

 Distributed as-is; no warranty is given.
******************************************************************************/
 
#ifndef __VKEY_H__
#define __VKEY_H__

#include <Arduino.h>

/**
 * A class to interface with a single VKey analog voltage keypad.
 * To interface with multiple keypads, instantiate multiple instance of this
 * class, each reading a unique analog input pin. 
 */
class VKey
{
  public:
  
    /* Definition of power supply/converter reference voltages
     */
    enum eVref
    {
      THREE, // 3.3V power supply and Vref
      FIVE,  // 5V  power supply and Vref
      MAX    // Do not use: used for range checking only
    };
    
    /* Enumerated values returned by the class to represent 
     * the current key combination.
     */
    enum eKeynum
    {
      // Keys map directly to regular integers
      KEY_NONE = 0,
      KEY_1,
      KEY_2,
      KEY_3,
      KEY_4,
      KEY_5,
      KEY_6,
      KEY_7,
      KEY_8,
      KEY_9,
      KEY_10,
      KEY_11,
      KEY_12
    };
    
    /* Constructor - used to create an instance of the VKey object.
     * 
     * Parameters:
     *    - pin: define which ADC pin the VKey is attached to.
     *    - reference: which voltage is powering the keypad, and being used 
     *      as the ADC reference?  (use the enum VKey::THREE or VKey::FIVE) 
     */
    VKey(uint8_t pin,
         eVref reference);
         
    /* CheckKeys - Return the currently pressed key (or KEY_NONE), and
     *            Indicate whether the reading has changed since the last poll
     *
     * This function can be used one of two ways.
     *
     * First, it can be used to simply query the current keypad status.  It will 
     * return the present key in the pass-by-reference parameter k.
     *
     * Second, it can be used to detect changed in keypad status.
     * The function returns a boolean value that
     * indicates whether the input has changed since the last query, then check 
     * the value of k and respond appropriately.  For example:
     *
     * Usage Example:
     *
     * #include <VKey.h>
     *
     * VKey keypad(0, FIVE);
     *
     * void setup() 
     * {
     *
     * }
     *
     * void loop() 
     * {
     *   eKeynum newKey;
     *
     *   if(keypad.CheckKey(newKey)
     *   {
     *      Serial.print("Got new key value: ");
     *      Serial.println(newkey);
     *   }
     * }
     * The responsiveness of the keypad depends on the rate the application 
     * calls this routine.
     */
    bool checkKeys(eKeynum & k);
    
  private:
    /* default constructor won't construct a valid object,
     * therefore it's hidden by making it private
     */
    VKey();
    
    /* Internal routine that takes an ADC value, and interpolates the 
     * corresponding key value.
     */
    eKeynum voltageToKey(int v);
    
    /* Internal structure to hold the calibration tables for the different
     * reference voltages
     */
    struct VKeyScale
    {
      PROGMEM const uint16_t offset;
      PROGMEM const uint16_t step_size;
      PROGMEM const uint16_t top;
    };

    /* Declaration of an array of Scale structures
     *
     * It's static because we can share one instance of the data with 
     * multiple VKeys.
     * Since it's constant, it's placed in PROGMEM to avoid using RAM.
     */
    static const PROGMEM VKeyScale scales[MAX];
    
    // Pointer to active row of the above table.
    const PROGMEM  VKeyScale* scale_p;
    
    /* Member variables
     */
          int      analog_pin;
          eKeynum  last_key;
};

#endif
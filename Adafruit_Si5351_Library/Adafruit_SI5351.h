/**************************************************************************/
/*!
    @file     Adafruit_SI5351.h
    @author   K. Townsend (Adafruit Industries)

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2014, Adafruit Industries
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _SI5351_H_
#define _SI5351_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif
//#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "errors.h"
#include "asserts.h"

#define SI5351_ADDRESS            (0x60) // Assumes ADDR pin = low
#define SI5351_READBIT            (0x01)

/* Test setup from SI5351 ClockBuilder
 * -----------------------------------
 * XTAL:      25     MHz
 * Channel 0: 120.00 MHz
 * Channel 1: 12.00  MHz
 * Channel 2: 13.56  MHz
 */
static const uint8_t m_si5351_regs_15to92_149to170[100][2] =
{
  {  15, 0x00 },    /* Input source = crystal for PLLA and PLLB */
  {  16, 0x4F },    /* CLK0 Control: 8mA drive, Multisynth 0 as CLK0 source, Clock not inverted, Source = PLLA, Multisynth 0 in integer mode, clock powered up */
  {  17, 0x4F },    /* CLK1 Control: 8mA drive, Multisynth 1 as CLK1 source, Clock not inverted, Source = PLLA, Multisynth 1 in integer mode, clock powered up */
  {  18, 0x6F },    /* CLK2 Control: 8mA drive, Multisynth 2 as CLK2 source, Clock not inverted, Source = PLLB, Multisynth 2 in integer mode, clock powered up */
  {  19, 0x80 },    /* CLK3 Control: Not used ... clock powered down */
  {  20, 0x80 },    /* CLK4 Control: Not used ... clock powered down */
  {  21, 0x80 },    /* CLK5 Control: Not used ... clock powered down */
  {  22, 0x80 },    /* CLK6 Control: Not used ... clock powered down */
  {  23, 0x80 },    /* CLK7 Control: Not used ... clock powered down */
  {  24, 0x00 },    /* Clock disable state 0..3 (low when disabled) */
  {  25, 0x00 },    /* Clock disable state 4..7 (low when disabled) */
  /* PLL_A Setup */
  {  26, 0x00 },
  {  27, 0x05 },
  {  28, 0x00 },
  {  29, 0x0C },
  {  30, 0x66 },
  {  31, 0x00 },
  {  32, 0x00 },
  {  33, 0x02 },
  /* PLL_B Setup */
  {  34, 0x02 },
  {  35, 0x71 },
  {  36, 0x00 },
  {  37, 0x0C },
  {  38, 0x1A },
  {  39, 0x00 },
  {  40, 0x00 },
  {  41, 0x86 },
  /* Multisynth Setup */
  {  42, 0x00 },
  {  43, 0x01 },
  {  44, 0x00 },
  {  45, 0x01 },
  {  46, 0x00 },
  {  47, 0x00 },
  {  48, 0x00 },
  {  49, 0x00 },
  {  50, 0x00 },
  {  51, 0x01 },
  {  52, 0x00 },
  {  53, 0x1C },
  {  54, 0x00 },
  {  55, 0x00 },
  {  56, 0x00 },
  {  57, 0x00 },
  {  58, 0x00 },
  {  59, 0x01 },
  {  60, 0x00 },
  {  61, 0x18 },
  {  62, 0x00 },
  {  63, 0x00 },
  {  64, 0x00 },
  {  65, 0x00 },
  {  66, 0x00 },
  {  67, 0x00 },
  {  68, 0x00 },
  {  69, 0x00 },
  {  70, 0x00 },
  {  71, 0x00 },
  {  72, 0x00 },
  {  73, 0x00 },
  {  74, 0x00 },
  {  75, 0x00 },
  {  76, 0x00 },
  {  77, 0x00 },
  {  78, 0x00 },
  {  79, 0x00 },
  {  80, 0x00 },
  {  81, 0x00 },
  {  82, 0x00 },
  {  83, 0x00 },
  {  84, 0x00 },
  {  85, 0x00 },
  {  86, 0x00 },
  {  87, 0x00 },
  {  88, 0x00 },
  {  89, 0x00 },
  {  90, 0x00 },
  {  91, 0x00 },
  {  92, 0x00 },
  /* Misc Config Register */
  { 149, 0x00 },
  { 150, 0x00 },
  { 151, 0x00 },
  { 152, 0x00 },
  { 153, 0x00 },
  { 154, 0x00 },
  { 155, 0x00 },
  { 156, 0x00 },
  { 157, 0x00 },
  { 158, 0x00 },
  { 159, 0x00 },
  { 160, 0x00 },
  { 161, 0x00 },
  { 162, 0x00 },
  { 163, 0x00 },
  { 164, 0x00 },
  { 165, 0x00 },
  { 166, 0x00 },
  { 167, 0x00 },
  { 168, 0x00 },
  { 169, 0x00 },
  { 170, 0x00 }
};

/* See http://www.silabs.com/Support%20Documents/TechnicalDocs/AN619.pdf for registers 26..41 */
enum
{
  SI5351_REGISTER_0_DEVICE_STATUS                       = 0,
  SI5351_REGISTER_1_INTERRUPT_STATUS_STICKY             = 1,
  SI5351_REGISTER_2_INTERRUPT_STATUS_MASK               = 2,
  SI5351_REGISTER_3_OUTPUT_ENABLE_CONTROL               = 3,
  SI5351_REGISTER_9_OEB_PIN_ENABLE_CONTROL              = 9,
  SI5351_REGISTER_15_PLL_INPUT_SOURCE                   = 15,
  SI5351_REGISTER_16_CLK0_CONTROL                       = 16,
  SI5351_REGISTER_17_CLK1_CONTROL                       = 17,
  SI5351_REGISTER_18_CLK2_CONTROL                       = 18,
  SI5351_REGISTER_19_CLK3_CONTROL                       = 19,
  SI5351_REGISTER_20_CLK4_CONTROL                       = 20,
  SI5351_REGISTER_21_CLK5_CONTROL                       = 21,
  SI5351_REGISTER_22_CLK6_CONTROL                       = 22,
  SI5351_REGISTER_23_CLK7_CONTROL                       = 23,
  SI5351_REGISTER_24_CLK3_0_DISABLE_STATE               = 24,
  SI5351_REGISTER_25_CLK7_4_DISABLE_STATE               = 25,
  SI5351_REGISTER_42_MULTISYNTH0_PARAMETERS_1           = 42,
  SI5351_REGISTER_43_MULTISYNTH0_PARAMETERS_2           = 43,
  SI5351_REGISTER_44_MULTISYNTH0_PARAMETERS_3           = 44,
  SI5351_REGISTER_45_MULTISYNTH0_PARAMETERS_4           = 45,
  SI5351_REGISTER_46_MULTISYNTH0_PARAMETERS_5           = 46,
  SI5351_REGISTER_47_MULTISYNTH0_PARAMETERS_6           = 47,
  SI5351_REGISTER_48_MULTISYNTH0_PARAMETERS_7           = 48,
  SI5351_REGISTER_49_MULTISYNTH0_PARAMETERS_8           = 49,
  SI5351_REGISTER_50_MULTISYNTH1_PARAMETERS_1           = 50,
  SI5351_REGISTER_51_MULTISYNTH1_PARAMETERS_2           = 51,
  SI5351_REGISTER_52_MULTISYNTH1_PARAMETERS_3           = 52,
  SI5351_REGISTER_53_MULTISYNTH1_PARAMETERS_4           = 53,
  SI5351_REGISTER_54_MULTISYNTH1_PARAMETERS_5           = 54,
  SI5351_REGISTER_55_MULTISYNTH1_PARAMETERS_6           = 55,
  SI5351_REGISTER_56_MULTISYNTH1_PARAMETERS_7           = 56,
  SI5351_REGISTER_57_MULTISYNTH1_PARAMETERS_8           = 57,
  SI5351_REGISTER_58_MULTISYNTH2_PARAMETERS_1           = 58,
  SI5351_REGISTER_59_MULTISYNTH2_PARAMETERS_2           = 59,
  SI5351_REGISTER_60_MULTISYNTH2_PARAMETERS_3           = 60,
  SI5351_REGISTER_61_MULTISYNTH2_PARAMETERS_4           = 61,
  SI5351_REGISTER_62_MULTISYNTH2_PARAMETERS_5           = 62,
  SI5351_REGISTER_63_MULTISYNTH2_PARAMETERS_6           = 63,
  SI5351_REGISTER_64_MULTISYNTH2_PARAMETERS_7           = 64,
  SI5351_REGISTER_65_MULTISYNTH2_PARAMETERS_8           = 65,
  SI5351_REGISTER_66_MULTISYNTH3_PARAMETERS_1           = 66,
  SI5351_REGISTER_67_MULTISYNTH3_PARAMETERS_2           = 67,
  SI5351_REGISTER_68_MULTISYNTH3_PARAMETERS_3           = 68,
  SI5351_REGISTER_69_MULTISYNTH3_PARAMETERS_4           = 69,
  SI5351_REGISTER_70_MULTISYNTH3_PARAMETERS_5           = 70,
  SI5351_REGISTER_71_MULTISYNTH3_PARAMETERS_6           = 71,
  SI5351_REGISTER_72_MULTISYNTH3_PARAMETERS_7           = 72,
  SI5351_REGISTER_73_MULTISYNTH3_PARAMETERS_8           = 73,
  SI5351_REGISTER_74_MULTISYNTH4_PARAMETERS_1           = 74,
  SI5351_REGISTER_75_MULTISYNTH4_PARAMETERS_2           = 75,
  SI5351_REGISTER_76_MULTISYNTH4_PARAMETERS_3           = 76,
  SI5351_REGISTER_77_MULTISYNTH4_PARAMETERS_4           = 77,
  SI5351_REGISTER_78_MULTISYNTH4_PARAMETERS_5           = 78,
  SI5351_REGISTER_79_MULTISYNTH4_PARAMETERS_6           = 79,
  SI5351_REGISTER_80_MULTISYNTH4_PARAMETERS_7           = 80,
  SI5351_REGISTER_81_MULTISYNTH4_PARAMETERS_8           = 81,
  SI5351_REGISTER_82_MULTISYNTH5_PARAMETERS_1           = 82,
  SI5351_REGISTER_83_MULTISYNTH5_PARAMETERS_2           = 83,
  SI5351_REGISTER_84_MULTISYNTH5_PARAMETERS_3           = 84,
  SI5351_REGISTER_85_MULTISYNTH5_PARAMETERS_4           = 85,
  SI5351_REGISTER_86_MULTISYNTH5_PARAMETERS_5           = 86,
  SI5351_REGISTER_87_MULTISYNTH5_PARAMETERS_6           = 87,
  SI5351_REGISTER_88_MULTISYNTH5_PARAMETERS_7           = 88,
  SI5351_REGISTER_89_MULTISYNTH5_PARAMETERS_8           = 89,
  SI5351_REGISTER_90_MULTISYNTH6_PARAMETERS             = 90,
  SI5351_REGISTER_91_MULTISYNTH7_PARAMETERS             = 91,
  SI5351_REGISTER_092_CLOCK_6_7_OUTPUT_DIVIDER          = 92,
  SI5351_REGISTER_165_CLK0_INITIAL_PHASE_OFFSET         = 165,
  SI5351_REGISTER_166_CLK1_INITIAL_PHASE_OFFSET         = 166,
  SI5351_REGISTER_167_CLK2_INITIAL_PHASE_OFFSET         = 167,
  SI5351_REGISTER_168_CLK3_INITIAL_PHASE_OFFSET         = 168,
  SI5351_REGISTER_169_CLK4_INITIAL_PHASE_OFFSET         = 169,
  SI5351_REGISTER_170_CLK5_INITIAL_PHASE_OFFSET         = 170,
  SI5351_REGISTER_177_PLL_RESET                         = 177,
  SI5351_REGISTER_183_CRYSTAL_INTERNAL_LOAD_CAPACITANCE	= 183
};

typedef enum
{
  SI5351_PLL_A = 0,
  SI5351_PLL_B,
} si5351PLL_t;

typedef enum
{
  SI5351_CRYSTAL_LOAD_6PF  = (1<<6),
  SI5351_CRYSTAL_LOAD_8PF  = (2<<6),
  SI5351_CRYSTAL_LOAD_10PF = (3<<6)
} si5351CrystalLoad_t;

typedef enum
{
  SI5351_CRYSTAL_FREQ_25MHZ = (25000000),
  SI5351_CRYSTAL_FREQ_27MHZ = (27000000)
} si5351CrystalFreq_t;

typedef enum
{
  SI5351_MULTISYNTH_DIV_4  = 4,
  SI5351_MULTISYNTH_DIV_6  = 6,
  SI5351_MULTISYNTH_DIV_8  = 8
} si5351MultisynthDiv_t;

typedef enum
{
  SI5351_R_DIV_1   = 0,
  SI5351_R_DIV_2   = 1,
  SI5351_R_DIV_4   = 2,
  SI5351_R_DIV_8   = 3,
  SI5351_R_DIV_16  = 4,
  SI5351_R_DIV_32  = 5,
  SI5351_R_DIV_64  = 6,
  SI5351_R_DIV_128 = 7,
} si5351RDiv_t;

typedef struct
{
  bool                initialised;
  si5351CrystalFreq_t crystalFreq;
  si5351CrystalLoad_t crystalLoad;
  uint32_t            crystalPPM;
  bool                plla_configured;
  uint32_t            plla_freq;
  bool                pllb_configured;
  uint32_t            pllb_freq;
} si5351Config_t;

class Adafruit_SI5351 
{
 public:
  Adafruit_SI5351(void);
  
  err_t begin(void);
  err_t setClockBuilderData(void);
  err_t setupPLL(si5351PLL_t pll, uint8_t mult, uint32_t num, uint32_t denom);
  err_t setupPLLInt(si5351PLL_t pll, uint8_t mult);
  err_t setupMultisynth(uint8_t output, si5351PLL_t pllSource, uint32_t div, uint32_t num, uint32_t denom);
  err_t setupMultisynthInt(uint8_t output, si5351PLL_t pllSource, si5351MultisynthDiv_t div);
  err_t enableOutputs(bool enabled);
  err_t setupRdiv(uint8_t  output, si5351RDiv_t div);

 private:
  si5351Config_t m_si5351Config;
  
  err_t write8(uint8_t reg, uint8_t value);
  err_t read8(uint8_t reg, uint8_t *value);  
};

#endif

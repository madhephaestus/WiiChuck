/*
 * Four Letter Word Generator
 * (C) 2012-3 Akafugu Corporation
 *
 * This program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 */

/*
 * To use this Four Letter Word generator you will need the following:
 *
 * - A 512kbit/64kb I2C EEPROM
 * - A database file, generated from this Processing application:
 *   https://github.com/perjg/fourletterword
 * - A method for uploading the data file to the EEPROM
 *   (Either an Arduino Mega, or a normal Arduino with a micro SD card)
 */

#include <Arduino.h>
#include <Wire.h>

#include <string.h>
#include "FourLetterWord.h"
#include "flw_blacklist.h"

#include <avr/pgmspace.h>

#define EEPROM_ADDR 0b1010000

void FourLetterWord::rot13(char* w)
{
    while (*w != '\0') {
        if (*w >= 'A' && *w <= 'M') {
            *w += 13;
        }
        else if (*w >= 'N' && *w <= 'Z') {
            *w -= 13;
        }

      w++;
    }
}

bool FourLetterWord::binary_search(const char *key, int imin, int imax)
{
  int pos;
  int cond = 0;
  char buf[5];

  while (imin <= imax) {
    pos = (imin+imax) / 2;
    
    strcpy_P(buf, (char*)pgm_read_word(&(flw_blacklist[pos])));
    rot13(buf);
    cond = strcmp(key, buf);
    
    if (cond == 0)   return true;
    else if (cond>0) imin = pos+1;
    else             imax = pos-1;
  }
  
  return false;
}


uint8_t FourLetterWord::read_byte(int device, unsigned int addr) {
  uint8_t rdata = 0xFF;
  
  Wire.beginTransmission(device);
  Wire.write((int)(addr >> 8)); // MSB
  Wire.write((int)(addr & 0xFF)); // LSB
  Wire.endTransmission();
  
  Wire.requestFrom(device,1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}

void FourLetterWord::read_buffer(int device, unsigned int addr, uint8_t *buffer, int length) {
  Wire.beginTransmission(device);
  Wire.write((int)(addr >> 8)); // MSB
  Wire.write((int)(addr & 0xFF)); // LSB
  Wire.endTransmission();
  
  Wire.requestFrom(device,length);

  int c = 0;
  for ( c = 0; c < length; c++ )
    if (Wire.available()) buffer[c] = Wire.read();
}


void FourLetterWord::begin(uint32_t seed, bool censored)
{
  m_lfsr = seed;
  m_censored = censored;
}

uint32_t FourLetterWord::randomize()
{
  // http://en.wikipedia.org/wiki/Linear_feedback_shift_register
  // Galois LFSR: taps: 32 31 29 1; characteristic polynomial: x^32 + x^31 + x^29 + x + 1 */
  m_lfsr = (m_lfsr >> 1) ^ (-(m_lfsr & 1u) & 0xD0000001u);
  return m_lfsr;  
}

bool FourLetterWord::hasEeprom()
{
   uint8_t b1 = read_byte(EEPROM_ADDR, 0); 
   uint8_t b2 = read_byte(EEPROM_ADDR, 1); 
   
   if (b1 == 65 && b2 == 66)
     return true;
   return false;
}

char* FourLetterWord::get_word_censored()
{
  char* w = get_word_uncensored();
  
  // assume a maximum of 5 censored words chosen in a row
  for (uint8_t i = 0; i < 5; i++) {
    if (binary_search(w, 0, BLACKLIST_SIZE)) { // censored
      w = get_word_uncensored();
    }
    else
      return w;
  }
  
  return w;
}

char* FourLetterWord::get_word_uncensored()
{
  unsigned char low = 0xFF, high = 0xFF;
  unsigned char count = 0;
  int next = 0;

  read_buffer(EEPROM_ADDR, m_offset, (uint8_t*)m_current_word, 5);
  count = m_current_word[4];
  m_current_word[4] = '\0';

  next = randomize() % count;
  m_offset += 5 + next * 2;

  high = read_byte(EEPROM_ADDR, m_offset++);
  low  = read_byte(EEPROM_ADDR, m_offset++);

  m_offset = (high << 8) | low;

  return m_current_word;
}

char* FourLetterWord::getWord()
{
  if (m_censored) return get_word_censored();
  return get_word_uncensored();
}



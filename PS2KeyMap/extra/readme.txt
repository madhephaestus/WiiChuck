  PS2KeyMap - Extension to PS2KeyAdvanced library
  Copyright (c) 2007 Free Software Foundation.  All right reserved.
  Written by Paul Carpenter, PC Services <sales@pcserviceselectronics.co.uk>
  Created September 2014
  Updated January 2016

  This is for a LATIN style keyboard.

  Assumption ONLY ONE keyboard connected to system

  Converts key codes from PS2KeyAdvanced library reads, to UTF-8 codes (with 
  CTRL,ALT....) taking into account the country keyboard mapping selected.

  Note as humans cannot type multiple keys at EXACT same time if waiting for 
  the three keys together <CTRL> + <ALT> + <DEL>, you WILL receive, mixtures of
  these keys before the final combination is seen.

  You can add your own keyboard mappings as EXTRA mappings, follow the style in
  PS2KeyData.h to add a mapping table. The mapping tables only contain the
  differences from US layout. Use PS2KeyAdvanced example SimpleTest to note the
  codes received first from the keyboard for the keys you want to change before 
  adding the table.

  See also file websites.txt for website information about PS2 interface, protocol
  scancodes and UTF-8 encoding.

  Files are

   extras folder
      readme.txt        this file
      websites.txt	    Other websites about UTF-8 and PS2 keyboard
      UTF-8codes.txt    Constants for codes beyond US-ASCII

   src folder
      PS2KeyMap.cpp     the library code
      PS2KeyMap.h       Library Header for sketches defines class and values
                        returned for keys
      PS2KeyData.h      Mapping tables (held in Flash)

   examples folder
      international     reads every returned keycode back to serial
                        Some keys will change the mapping to different country 
                        on the fly
      KeyToLCD          reads keyboard and displays where possible on LCD with 
                        pre-selected in code ONE country mapping

  Reading a key code returns an UNSIGNED INT containing
        Make/Break status
        Caps status
        Shift, Ctrl, Alt, Alt Gr, GUI keys
        Flag for function key not a displayable/printable character
        8 bit key code

  See PS2KeyAdvanced for non-UTF-8 mapping codes like F1 to F24, HOME, END as
  these will set the function bit.
  
  See extras/UTF-8codes.txt for the codes returned beyond standard US-ASCII in
  bottom 8 bits

  Top Byte is 8 bits denoting as follows and standard defines available see 
  PS2KeyAdvanced for these details

  Error Codes
     Most functions return 0 or 0xFFFF as error, other codes to note and
     handle appropriately

Paul Carpenter
PC Services
January 2016
http://www.pcserviceselectronics.co.uk
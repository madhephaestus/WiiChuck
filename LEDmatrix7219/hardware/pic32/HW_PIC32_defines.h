// *** Hardwarespecific defines ***
#define __cbi(reg, bitmask) (*(reg + 1)) = bitmask
#define __sbi(reg, bitmask) (*(reg + 2)) = bitmask
#define pulseClock __cbi(P_CLOCK, B_CLOCK); asm ("nop"); __sbi(P_CLOCK, B_CLOCK)

#define fontbyte(x) cfont.font[x]  

#define PROGMEM
#define regtype volatile uint32_t
#define regsize volatile uint16_t


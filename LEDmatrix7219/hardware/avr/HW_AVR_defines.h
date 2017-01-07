// *** Hardwarespecific defines ***
#define __cbi(reg, bitmask) *reg &= ~bitmask
#define __sbi(reg, bitmask) *reg |= bitmask
#define pulseClock __cbi(P_CLOCK, B_CLOCK); asm ("nop"); __sbi(P_CLOCK, B_CLOCK)

#define fontbyte(x) pgm_read_byte(&cfont.font[x])  

#define regtype volatile uint8_t
#define regsize volatile uint8_t

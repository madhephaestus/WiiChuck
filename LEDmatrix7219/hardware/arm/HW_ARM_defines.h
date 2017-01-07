// *** Hardwarespecific defines ***
#define __cbi(reg, bitmask) *reg &= ~bitmask
#define __sbi(reg, bitmask) *reg |= bitmask
#define pulseClock __cbi(P_CLOCK, B_CLOCK); asm ("nop"); __sbi(P_CLOCK, B_CLOCK)

#define fontbyte(x) cfont.font[x]  

#define regtype volatile uint32_t
#define regsize volatile uint32_t

#define sei() interrupts()
#define cli() noInterrupts()
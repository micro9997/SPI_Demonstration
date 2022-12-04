
#ifndef __OUT_PWM__
#define __OUT_PWM__

#include <avr/io.h>

#define F_CPU 16000000

#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

void initPWM_PB1();
void writePWM_PB1(uint8_t data);

#endif

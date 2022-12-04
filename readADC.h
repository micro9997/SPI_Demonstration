
#ifndef __read_ADC0__
#define __read_ADC0__

#include <avr/io.h>

#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

void initADC0();
uint16_t readADC0(void);

#endif

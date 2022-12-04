
#include "writePWM.h"

void initPWM_PB1() {
    // Setup PB1 as output
    DDRB |= (1 << PORTB1);

    // Setup to fast PWM, 8-bit
    TCCR1A |= (1 << WGM10);
    TCCR1B |= (1 << WGM12);

    // Setup to clk I/O/8 (From prescaler)
    TCCR1B |= (1 << CS11);

    // Setup to Clear OC1A/OC1B on Compare Match (Set output to low level) for OCR1A - PB1.
    TCCR1A |= (1 << COM1A1);

    // PWM value zero 
    OCR1A = 0;
}

void writePWM_PB1(uint8_t data) {
    // Store the data to reg
    OCR1A = data;
}

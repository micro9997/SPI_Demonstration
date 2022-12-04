
#include "readADC.h"

void initADC0() {
    // AV CC with external capacitor at AREF pin
    ADMUX |= (1 << REFS0);

    // Setup MUX for Use ADC0 as a analog input
    // ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));

    // Setup to Prescaler /8
    ADCSRA |= (1 << ADPS1) | (1 << ADPS0);

    // ADC Enable
    ADCSRA |= (1 << ADEN);
}

uint16_t readADC0(void) {
    // ADC Start Conversion
    ADCSRA |= (1 << ADSC);

    loop_until_bit_is_clear(ADCSRA, ADSC);

    return ADC;
}

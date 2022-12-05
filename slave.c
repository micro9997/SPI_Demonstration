
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#include "writePWM.h"

void SPI_SlaveInit(void) {
    // Set MISO output, all others input
    DDRB |= (1 << PORTB4);

    // Enable SPI interrupt and SPI
    SPCR |= ((1 << SPIE) | (1 << SPE));

    // Enable global interrupt
    SREG |= (1 << 7);
}

uint8_t SPI_SlaveReceive(void) {
    // Wait for reception complete
    while(!(SPSR & (1 << SPIF)));

    // Return Data Register
    return SPDR;
}

ISR(SPI_STC_vect) {
    writePWM_PB1(SPDR);
}

int main() {
    SPI_SlaveInit();
    initPWM_PB1();

    while(1) {

    }

    return 0;
}

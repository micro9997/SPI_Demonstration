
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#include <avr/io.h>

#include "readADC.h"

void SPI_MasterInit(void) {
    // Set SCK, MOSI and SS output, all others input
    DDRB |= ((1 << PORTB5) | (1 << PORTB3) | (1 << PORTB2));

    // SS to high
    PORTB |= (1 << PORTB2);

    // Enable SPI, Master, set clock rate fck/16
    SPCR |= ((1 << SPE) | (1 << MSTR) | (1 << SPR0));
}

void SPI_MasterTransmit(uint8_t cData) {
    // SS low
    PORTB &= ~(1 << PORTB2);

    // Start transmission
    SPDR = cData;

    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));

    // SS high
    PORTB |= (1 << PORTB2);
}

int main() {
    SPI_MasterInit();
    initADC0();

    uint8_t adcData = 0;
    uint8_t adcDataPre = 0;

    while(1) {
        adcData = readADC0() / 4;
        if(adcData != adcDataPre) {
            SPI_MasterTransmit(adcData);
            adcDataPre = adcData;
        }
    }

    return 0;
}

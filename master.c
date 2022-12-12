
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

void SPI_MasterInit(void) {
    // Set SCK, MOSI and SS output, all others input
    DDRB |= ((1 << PORTB5) | (1 << PORTB3) | (1 << PORTB2));
    // SS to high
    PORTB |= (1 << PORTB2);
    // Enable SPI, Master, set clock rate fck/128
    SPCR |= ((1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0));
}

uint8_t readButtonData() {
    PORTB &= ~(1 << PORTB2);
    // Identifier for Request
    SPDR = 0b00001000;
    while(!(SPSR & (1 << SPIF)));
    // XXXX
    SPDR = 0b00000000;
    while(!(SPSR & (1 << SPIF)));
    // _delay_ms(100);
    // Dummy for receive data
    SPDR = 0b00000000;
    while(!(SPSR & (1 << SPIF)));
    // _delay_ms(100);
    // Dummy for receive data
    SPDR = 0b00000000;
    while(!(SPSR & (1 << SPIF)));
    PORTB |= (1 << PORTB2);

    return SPDR;
}

int main() {
    SPI_MasterInit();
    DDRB |= (1 << PORTB1);
    PORTB &= ~(1 << PORTB1);

    int receiveData;

    while(1) {
        receiveData = readButtonData();
        if(receiveData == 0b00000001) {
            PORTB |= (1 << PORTB1);
        } else {
            PORTB &= ~(1 << PORTB1);
        }

        _delay_ms(1000);
    }

    return 0;
}

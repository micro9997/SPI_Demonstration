
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

#include "readADC.h"

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

    _delay_ms(1);

    // Dummy for receive data
    SPDR = 0b00000000;
    while(!(SPSR & (1 << SPIF)));

    PORTB |= (1 << PORTB2);

    return SPDR;
}

void sendADC_Data(uint8_t data) {
    // SS low
    PORTB &= ~(1 << PORTB2);
    // Start transmission
    SPDR = 0b00000100;
    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));
    // Start transmission
    SPDR = data;
    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));
    // SS high
    PORTB |= (1 << PORTB2);
}

int main() {
    SPI_MasterInit();
    initADC0();
    DDRB |= (1 << PORTB1);
    PORTB &= ~(1 << PORTB1);

    uint8_t receiveData;
    uint8_t sendData = 0;
    uint8_t sendDataPre = 0;

    while(1) {
        receiveData = readButtonData();
        if(receiveData == 0b00000001) {
            PORTB |= (1 << PORTB1);
        } else {
            PORTB &= ~(1 << PORTB1);
        }

        sendData = readADC0() / 4;
        if(sendData != sendDataPre) {
            sendADC_Data(sendData);
            sendDataPre = sendData;
        }
    }

    return 0;
}

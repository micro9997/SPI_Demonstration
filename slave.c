
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#include "writePWM.h"

uint8_t buttonData = 1;

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
    static uint16_t data = 0;
    static uint8_t dataLen = 0;
    static uint8_t needToSend = 0;

    if(needToSend == 1) {
        SPDR = buttonData;
        needToSend = 0;
    } else {
        dataLen++;
        if(dataLen == 1) {
            data = SPDR;
            SPDR = 0;
        } else if(dataLen == 2) {
            data = (data << 8) | SPDR;
            SPDR = 0;

            if((data & 0b100000000) != 0) {
                writePWM_PB1((data & 0b11111111));
            } else if((data & 0b1000000000) != 0) {
                SPDR = 0b00000100;
                needToSend = 1;
            }

            dataLen = 0;
            data = 0;
        }
    }
}

int main() {
    SPI_SlaveInit();
    initPWM_PB1();

    DDRD &= ~(1 << PIND5);

    while(1) {
        /* if(((PIND & (1 << PIND5)) != 0) && (buttonData == 0)) {
            buttonData = 1;

        } else if(((PIND & (1 << PIND5)) == 0) && (buttonData == 1)) {
            buttonData = 0;

        } */
    }

    return 0;
}


#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t buttonDataFlag = 0;
uint8_t buttonData = 0;

void SPI_SlaveInit(void) {
    // Set MISO output, all others input
    DDRB |= (1 << PORTB4);
    // Enable SPI interrupt and SPI
    SPCR |= ((1 << SPIE) | (1 << SPE));
    // Enable global interrupt
    SREG |= (1 << 7);
}

ISR(SPI_STC_vect) {
    static uint16_t data = 0;
    static uint8_t numOfByte = 0;
    static uint8_t needToSend = 0;

    if(needToSend == 1) {
        SPDR = buttonData;
        needToSend = 2;

    } else if(needToSend == 2) {
        needToSend = 0;

    } else {
        numOfByte++;
        if(numOfByte == 1) {
            data = SPDR;

        } else if(numOfByte == 2) {
            data = (data << 8) | SPDR;
            if((data & 0b1111111100000000) == 0b0000010000000000) { // <--- Identifier with Data
                // Slave Receive the Data

            } else if(data == 0b0000100000000000) { // <--- Request data
                // Slave Send the Data
                SPDR = 0b00000000;
                needToSend = 1;
            }

            numOfByte = 0;
        }
    }
}

int main() {
    SPI_SlaveInit();
    DDRD &= ~(1 << PIND5);
    DDRB |= (1 << PORTB1);
    PORTB &= ~(1 << PORTB1);

    while(1) {
        if(((PIND & (1 << PIND5)) != 0) && (buttonDataFlag == 0)) {
            buttonData ^= (1 << 0);
            PORTB ^= (1 << PORTB1);
            buttonDataFlag = 1;

            // SPDR = buttonData;

        } else if(((PIND & (1 << PIND5)) == 0) && (buttonDataFlag == 1)) {
            buttonDataFlag = 0;

        }
    }

    return 0;
}

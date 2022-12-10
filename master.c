
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#include "readADC.h"

void SPI_MasterInit(void) {
    // Set SCK, MOSI and SS output, all others input
    DDRB |= ((1 << PORTB5) | (1 << PORTB3) | (1 << PORTB2));
    // SS to high
    PORTB |= (1 << PORTB2);
    // Enable SPI, Master, set clock rate fck/16
    SPCR |= ((1 << SPE) | (1 << MSTR) | (1 << SPR0));
}

void initLED() {
    // Set LED pin as output
    DDRB |= (1 << PORTB1);
    PORTB &= ~(1 << PORTB1);
}

void writeLED(uint8_t data) {
    if(data == 0b00000001) {
        PORTB |= (1 << PORTB1);
    } else {
        PORTB &= ~(1 << PORTB1);
    }
}

void sendADC_Data(uint8_t data) {
    // SS low
    PORTB &= ~(1 << PORTB2);
    // Start transmission
    SPDR = 0b00000001;
    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));
    // Start transmission
    SPDR = data;
    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));
    // SS high
    PORTB |= (1 << PORTB2);
}

int8_t readButtonData() {
    uint8_t correctDataFlag = 0;
    // Declare the return data
    uint8_t data = 0;
    // SS low
    PORTB &= ~(1 << PORTB2);
    // Start transmission
    SPDR = 0b00000010;
    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));
    // Start transmission
    SPDR = 0b00000000;
    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));

    // 
    SPDR = 0b00000000;
    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));
    // Validate the data
    if(SPDR == 0b00000100) {
        correctDataFlag = 1;
    }
    // 
    SPDR = 0b00000000;
    // Wait for transmission complete
    while(!(SPSR & (1 << SPIF)));
    // 
    data = SPDR;

    // SS high
    PORTB |= (1 << PORTB2);

    if(correctDataFlag == 1) {
        return data;
    } else {
        return -1;
    }
}

int main() {
    SPI_MasterInit();
    initADC0();
    initLED();

    uint8_t adcData = 0;
    uint8_t adcDataPre = 0;
    uint16_t buttonData = 0;

    while(1) {
        adcData = readADC0() / 4;
        if(adcData != adcDataPre) {
            sendADC_Data(adcData);
            adcDataPre = adcData;
        }
        
        buttonData = readButtonData();
        if(buttonData != -1) {
            // writeLED(buttonData);
            PORTB |= (1 << PORTB1);
        } else {
            // Error in SPI communication
            // PORTB |= (1 << PORTB1);
        }
    }

    return 0;
}

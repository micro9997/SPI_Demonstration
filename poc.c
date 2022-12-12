
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#include <avr/io.h>

uint16_t readMCP3008() {
    // Read channel 0 and return value

    uint8_t LSB;
    uint8_t MSB;

    // Holds data to return to main code
    uint16_t ADCvalue;

    // Set up SPI bus for MCP3008
    SPCR = 0b01010010;

    // SS on PB1 (15)

    // SS pin LOW
    PORTB &= ~(1 << PORTB1);

    // Send start bit
    SPDR = 0b00000001;

    // Wait for SPI transmission to finish
    while(!(SPSR & (1 << SPIF)));

    // Select ADC0
    SPDR = 0b10000000;

    while(!(SPSR & (1 << SPIF)));

    // Place top 2 bits of ADC value in MSB, ignore unwanted bits
    MSB = SPDR & 0b00000011;

    // Request next 8 bits of data
    SPDR = 0b00000000;

    while(!(SPSR & (1 << SPIF)));

    // Place lower 8 bits of data in LSB
    LSB = SPDR;

    // SS pin HIGH
    PORTB |= (1 << PORTB1);

    // Construct final 10-bit ADC value
    ADCvalue = MSB << 8 | LSB;

    return ADCvalue;
}

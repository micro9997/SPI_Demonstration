
&nbsp;

### AVR AND THE SPI BUS

&nbsp;

#### Project 44 - Using the 74HC595 Shift Register

```C
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

void setupSPI() {
    // SS pin HIGH
    PORTB |= (1 << 0);

    // Set up SPI bus
    SPCR = 0b01110000;
}

void dispNumSR(uint8_t value) {
    // Displays a number from 0–9 on the seven-segment LED display

    // SS pin LOW
    PORTB &= ~(1 << PORTB0);
    
    switch(value) {
        // Determine which byte of data to send to the 74HC595
        case 0 : SPDR = 0b11111100; break; // 0
        case 1 : SPDR = 0b01100000; break; // 1
        case 2 : SPDR = 0b11011010; break; // 2
        case 3 : SPDR = 0b11110010; break; // 3
        case 4 : SPDR = 0b01100110; break; // 4
        case 5 : SPDR = 0b10110110; break; // 5
        case 6 : SPDR = 0b10111110; break; // 6
        case 7 : SPDR = 0b11100000; break; // 7
        case 8 : SPDR = 0b11111110; break; // 8
        case 9 : SPDR = 0b11100110; break; // 9
    }

    // Wait for SPI transmission to finish
    while(!(SPSR & (1 << SPIF)));
    
    // SS pin HIGH
    PORTB |= (1 << PORTB0);
}

int main(void) {
    uint8_t i = 0;

    // Set PORTB as outputs
    DDRB = 0b11111111;

    setupSPI();

    while(1) {
        for(i = 0; i < 10; i++) {
            dispNumSR(i);
            _delay_ms(250);
        }
    }

    return 0;
}
```

&nbsp;

#### Project 45 - Using Two 74HC595 Shift Registers
```C
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

void setupSPI() {
    // SS pin HIGH
    PORTB |= (1 << 0);

    // Set up SPI bus
    SPCR = 0b01110000;
}

void dispNumSR(uint8_t value) {
    // Displays a number from 00–99 on the seven-segment LED displays
    uint8_t leftDigit;
    uint8_t rightDigit;
    uint8_t digitData[] = {0b11111100, 0b01100000,
                            0b11011010, 0b11110010,
                            0b01100110, 0b10110110,
                            0b10111110, 0b11100000,
                            0b11111110, 0b11100110};
    leftDigit = value / 10;
    rightDigit = value % 10;

    // SS pin LOW
    PORTB &= ~(1 << PORTB0);

    SPDR = digitData[rightDigit];

    // Wait for SPI transmission to finish
    while(!(SPSR & (1 << SPIF)));

    SPDR = digitData[leftDigit];

    // Wait for SPI transmission to finish
    while(!(SPSR & (1 << SPIF)));

    // SS pin HIGH
    PORTB |= (1 << PORTB0);
}

int main(void) {
    uint8_t i = 0;

    // Set PORTB as outputs
    DDRB = 0b11111111;
    
    setupSPI();
    
    while(1) {
        for(i = 0; i < 100; i++) {
            dispNumSR(i);
            _delay_ms(250);
        }
    }
}
```

&nbsp;

#### Project 46 - Using the MAX7219 LED Driver IC 

```C
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

void writeMAX7219(char hexdata1, char hexdata2) {
    // Sends two bytes in hexadecimal to the MAX7219

    // SS pin LOW
    PORTB &= ~(1 << PORTB0);

    // Send value of hexdata1
    SPDR = hexdata1;

    // Wait for SPI transmission to finish
    while(!(SPSR & (1 << SPIF)));

    // Send value of hexdata2
    SPDR = hexdata2;

    // Wait for SPI transmission to finish
    while(!(SPSR & (1 << SPIF)));

    // SS pin HIGH
    PORTB |= (1 << PORTB0);
}

void blankMAX7219() {
    // Blanks all digits

    uint8_t i;

    // Blank all digits
    for(i = 1; i < 9; i++) {
        // Send blank (15) to digit register (i)
        writeMAX7219(i, 15);
    }
}

void initMAX7219() {
    // Set up MAX7219 for use

    // SS pin HIGH
    PORTB |= (1 << 0);

    // Set up SPI bus for MAX7219
    SPCR = 0b01010000;

    // Mode decode for digits
    writeMAX7219(0x09, 0xFF);

    // Set scan limit to 8 digits: 0x09 + 0xFF)
    writeMAX7219(0x0B, 0x07);

    // Set intensity to 8 - 0x0A + 0x08)
    writeMAX7219(0x0A, 0x01);

    // Mode display on
    writeMAX7219(0x0C, 0x01);

    blankMAX7219();
}

void dispMAX7219(uint8_t digit, uint8_t number, uint8_t dp) {
    // Displays "number" in location "digit" with decimal point on/off
    // Digit: 1~8 for location 1~8
    // Number: 0~15 for 0~9, - E, H, L, P, blank
    // dp: 1 on, 0 off

    // Add decimal point
    if(dp == 1) {
        number = number + 128;
    }

    writeMAX7219(digit, number);
}

void numberMAX7219(uint32_t value) {
    // Displays a number between 0–99999999

    uint8_t digits[9];
    uint8_t i = 1;

    for(i = 1; i < 9; i++) {
        // Sending 15 blanks the digit
        digits[i] = 15;
    }
    i = 1;

    // Continue until value > 0
    while(value > 0) {
        // Determine and store last digit
        digits[i] = value % 10;
        // Divide value by 10
        value = value / 10;
        i++;
    }

    for(i = 1; i < 9; i++) {
        dispMAX7219(i, digits[i], 0);
    }
}

int main(void) {
    uint32_t i;

    // Set PORTB as outputs
    DDRB = 0b11111111;

    initMAX7219();

    while(1) {
        for(i = 0; i < 100000000; i++) {
            numberMAX7219(i);
            _delay_ms(100);
        }
    }
}
```

&nbsp;

#### Project 48 - Using the MCP3008 ADC IC

```C
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

void writeMAX7219(char hexdata1, char hexdata2) {
    // Sends two bytes in hexadecimal to MAX7219

    // Set up SPI bus for MAX7219
    SPCR = 0b01010000;

    // SS pin LOW
    PORTB &= ~(1 << PORTB0);

    // Send value of hexdata1
    SPDR = hexdata1;

    // Wait for SPI transmission to finish
    while(!(SPSR & (1 << SPIF)));

    // Send value of hexdata2
    SPDR = hexdata2;

    // Wait for SPI transmission to finish
    while(!(SPSR & (1 << SPIF)));

    // SS pin HIGH
    PORTB |= (1 << PORTB0);
}

void blankMAX7219() {
    uint8_t i;
    for(i = 1; i < 9; i++) {
        writeMAX7219(i, 15);
    }
}

void initMAX7219() {
    // Set up MAX7219 for use

    PORTB |= (1 << 0);

    SPCR = 0b01010000;

    writeMAX7219(0x09, 0xFF);
    writeMAX7219(0x0B, 0x07);
    writeMAX7219(0x0A, 0x01);
    writeMAX7219(0x0C, 0x01);

    blankMAX7219();
}

void dispMAX7219(uint8_t digit, uint8_t number, uint8_t dp) {
    // Add decimal point
    if(dp == 1) {
        number = number + 128;
    }
    writeMAX7219(digit, number);
}

void numberMAX7219(uint32_t value) {
    // Displays a number between 0–99999999

    uint8_t digits[9];
    uint8_t i = 1;

    for(i = 1; i < 9; i++) {
        // Sending 15 blanks the digit
        digits[i] = 15;
    }
    i = 1;

    // Continue until value > 0
    while(value > 0) {
        // Determine and store last digit
        digits[i] = value % 10;

        // Divide value by 10
        value = value / 10;
        i++;
    }

    for(i = 1; i < 9; i++) {
        dispMAX7219(i, digits[i], 0);
    }
}

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

int main(void) {
    uint16_t ADCoutput;

    // Set PORTB as outputs
    DDRB = 0b11111111;

    initMAX7219();

    while(1) {
        ADCoutput = readMCP3008();

        // Convert ADC value to millivolts
        ADCoutput = ADCoutput * 4.8828;

        numberMAX7219(ADCoutput);

        _delay_ms(100);
    }
}
```

&nbsp;


# SPI_Demonstration
SPI Demonstration using ATmega328P

&nbsp;

### 01. Introduction
*The Serial Peripheral Interface (SPI) allows high-speed synchronous data transfer between the ATmega328P and peripheral devices or between several AVR devices.*

*Features*
* *Full-duplex communication*
* *Synchronous data transfer*
* *Master / slave operation*

*The interconnection between Master and Slave CPUs with SPI. The system consists of two shift Registers, and a Master clock generator. The SPI Master initiates the communication cycle when pulling low the Slave Select SS pin of the desired Slave. Master and Slave prepare the data to be sent in their respective shift Registers, and the Master generates the required clock pulses on the SCK line to interchange data. Data is always shifted from Master to Slave on the Master Out – Slave In, MOSI, line, and from Slave to Master on the Master In – Slave Out, MISO, line. After each data packet, the Master will synchronize the Slave by pulling high the Slave Select, SS, line.*

&nbsp;

### 02. Methodology

![image_1](https://github.com/micro9997/SPI_Demonstration/blob/master/images/image_1.jpeg)

&nbsp;

**Register Description**

* *SPCR – SPI Control Register*
    * *Bit 7 – SPIE: SPI Interrupt Enable*
    * *Bit 6 – SPE: SPI Enable*
    * *Bit 5 – DORD: Data Order*
    * *Bit 4 – MSTR: Master/Slave Select*
    * *Bit 3 – CPOL: Clock Polarity*
    * *Bit 2 – CPHA: Clock Phase*
    * *Bits 1, 0 – SPR1, SPR0: SPI Clock Rate Select 1 and 0*

* *SPSR – SPI Status Register*
    * *Bit 7 – SPIF: SPI Interrupt Flag*
    * *Bit 6 – WCOL: Write COLlision Flag*
    * *Bit [5:1] – Reserved*
    * *Bit 0 – SPI2X: Double SPI Speed Bit*

* *SPDR – SPI Data Register*

&nbsp;

*Pin initialization*
```C
// for Master

// Set SCK, MOSI and SS output, all others input
DDR_SPI = ((1 << DD_SCK) | (1 << DD_MOSI) | (1 << SS));
// SS to high
PORTB |= (1 << PORTB2);
```

```C
// for Slave

// Set MISO output, all others input
DDR_SPI = (1 << DD_MISO);
```

&nbsp;

*Enable the SPI peripheral*
```C
// for Master

// Enable SPI, Master, set clock rate fck/128
SPCR = ((1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0));
```

```C
// for Slave

// Enable SPI interrupt and SPI
SPCR |= ((1 << SPIE) | (1 << SPE));
// Enable global interrupt
SREG |= (1 << 7);
```

&nbsp;

*Handle the data*
```C
// for Master (Send the data)

// Start transmission
SPDR = cData;
// Wait for transmission complete
while(!(SPSR & (1 << SPIF)));
```

```C
// for Slave (Receive the data) with polling 

// Wait for reception complete
while(!(SPSR & (1 << SPIF)));
// Return Data Register
return SPDR;
```

```C
// for Slave (Receive the data) with interrupt

ISR(SPI_STC_vect) {

    data = SPDR;
}
```

&nbsp;

### 03. Evaluation and Result

[![image_2](https://github.com/micro9997/SPI_Demonstration/blob/master/images/image_2.png)](https://youtu.be/IfVZkUeRu1Y)

&nbsp;

### 04. Conclusion

Advantages
* It is a high-speed communication system capable of speeds of up to 8 Mbps.
* It's full-duplex communication.
* It doesn't contain data over heads unlike I2C data frame.
* It doesn't contain pull-up resistors in the physical connection.
* It's a low-power communication method. Other slaves are in sleep or standby mode if a slave is communicating with the master at that time.

Disadvantages
* Require more physical connections for this communication.
* It doesn't support multi-master communication.
* More GPIOs are needed for communicating with more slaves.

&nbsp;

### 05. Appendix

*master.c*
```C
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
```

*slave.c*
```C
#ifndef __AVR_ATmega328P__
    #include <avr/iom328p.h>
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#include "writePWM.h"

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
    static uint8_t wait = 0;

    if(wait == 0) {
        numOfByte++;
        if(numOfByte == 1) {
            data = SPDR;

        } else if(numOfByte == 2) {
            data = (data << 8) | SPDR;

            if((data & 0b1111111100000000) == 0b0000010000000000) { // <--- Identifier with Data
                // Slave Receive the Data
                writePWM_PB1(data & 0b11111111);

            } else if(data == 0b0000100000000000) { // <--- Request data
                // Slave Send the Data
                SPDR = buttonData;
                wait = 1;
            }

            numOfByte = 0;
        }
    } else {
        wait = 0;
    }
}

int main() {
    SPI_SlaveInit();
    initPWM_PB1();
    DDRD &= ~(1 << PIND5);
    DDRB |= (1 << PORTB1);
    PORTB &= ~(1 << PORTB1);

    while(1) {
        if(((PIND & (1 << PIND5)) != 0) && (buttonDataFlag == 0)) {
            buttonData ^= (1 << 0);
            // PORTB ^= (1 << PORTB1);
            buttonDataFlag = 1;

            // SPDR = buttonData;

        } else if(((PIND & (1 << PIND5)) == 0) && (buttonDataFlag == 1)) {
            buttonDataFlag = 0;

        }
    }

    return 0;
}
```

&nbsp;

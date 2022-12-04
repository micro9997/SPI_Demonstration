# SPI_Demonstration
SPI Demonstration using ATmega328P

&nbsp;

### 01. Introduction
*The Serial Peripheral Interface (SPI) allows high-speed synchronous data transfer between the ATmega328P and peripheral devices or between several AVR devices.*

*Features*
* *Full-duplex, Three-wire Synchronous Data Transfer*
* *Master or Slave Operation*
* *LSB First or MSB First Data Transfer*

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

// Set MOSI and SCK output, all others input
DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK);
```

```C
// for Slave

// Set MISO output, all others input
DDR_SPI = (1 << DD_MISO);
```

&nbsp;

*Peripheral enable*
```C
// for Master

// Enable SPI, Master, set clock rate fck/16
SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
```

```C
// for Slave

// Enable SPI
SPCR = (1 << SPE);
```

&nbsp;

*Handle data*
```C
// for Master

// Start transmission
SPDR = cData;
// Wait for transmission complete
while(!(SPSR & (1 << SPIF)));
```

```C
// for Slave

// Wait for reception complete
while(!(SPSR & (1 << SPIF)));
// Return Data Register
return SPDR;
```

&nbsp;

### 03. Evaluation and Result

&nbsp;

### 04. Conclusion

Advantages
* Need to write . .
* 

Disadvantages
* Need to write . .
* 

&nbsp;

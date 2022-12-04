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

### 03. Evaluation and Result

&nbsp;

### 04. Conclusion

&nbsp;

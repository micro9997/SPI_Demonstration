
avr-gcc -mmcu=atmega328p master.c readADC.c writePWM.c
avr-objcopy -O ihex a.out master.hex

rm a.out

avr-gcc -mmcu=atmega328p slave.c readADC.c writePWM.c
avr-objcopy -O ihex a.out slave.hex

rm a.out


avr-gcc -mmcu=atmega328p master.c readADC.c writePWM.c
avr-objcopy -O ihex a.out out.hex

rm a.out

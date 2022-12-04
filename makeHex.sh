
avr-gcc -mmcu=atmega328p node_2.c readADC.c writePWM.c
avr-objcopy -O ihex a.out out.hex

rm a.out

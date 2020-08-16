#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

void main() {
  DDRB = 0x1;
  
  while(1) {
    PORTB |= 0x1;
    _delay_ms(200);
    PORTB &= 0x0;
    _delay_ms(200);
  }
}

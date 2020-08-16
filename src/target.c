#ifndef F_CPU
#define F_CPU 1000
#endif

#include <avr/io.h>
#include <util/delay.h>

void main() {
  DDRB = 0b10;
  
  while(1) {
    PORTB |= 0b10;
    _delay_ms(1000);
    PORTB &= 0b00;
    _delay_ms(1000);
  }
}

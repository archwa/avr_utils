#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <target_spi_bin.h>
#include <spi_instr.h>

void main() {
  DDRB = 0b101;
  
  do_spi(target_spi_bin, target_spi_bin_len);

  while(1){}
}

int set_port_data(unsigned int mosi, unsigned int sck) {
  // PORTXn: (2)SCK, (1)MISO, (0)MOSI (+ add RESET and SS somewhere? hm.)
  PORTB = (sck << 2) | mosi;

  return 0;
}

// convert this to support USART comms with PC
// MCU programming only has so much memory
int spi_data_xfer(unsigned int mosi) {
  // artifical clock period: (1000 / F_SCK) ms
  unsigned int pulse_width = 500 / F_SCK;

  set_port_data(mosi, 0);
  _delay_ms(pulse_width);
  set_port_data(mosi, 1);
  _delay_ms(pulse_width);
  set_port_data(mosi, 0);

  return PINB & 0x2; // return MISO data
}

int do_spi(unsigned char* data, unsigned int data_len) {
  // data length must be a multiple of 4 (i.e., cannot have fractional instruction)
  if(data_len % 4) {
    return -1;
  }

  unsigned int instr_idx, i, j, mosi, miso;
  for(instr_idx = 0; instr_idx < data_len / 4; instr_idx++) {
    for(i = 0; i < 4; i++) { // instruction byte (of 4) index
      for(j = 8; j > 0; j--) { // bit
        mosi = (data[instr_idx*4 + i] & (1 << (j - 1))) > 0 ? 1 : 0;
        miso = spi_data_xfer(mosi); // contains miso

        // put extra stuff
      }
    }
  }

  return 0;
}

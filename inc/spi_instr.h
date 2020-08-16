#ifndef F_SCK
#define F_SCK 250
#endif

int set_port_data(unsigned int, unsigned int);
int spi_data_xfer(unsigned int);
int do_spi(unsigned char*, unsigned int);

/*
// only used if RDY/BSYb is not polled:
time_wait_flash_ms = 6  // true value 4.5ms
time_wait_eeprom_ms = 5 // true value 3.6ms
time_wait_erase_ms = 10 // true value 9.0ms


SPI memory programming algorithm

1. reset device (supply LOW to HIGH to LOW at target reset)
2. wait at least 20ms, then send Programming Enable serial instruction (temporal order: MSB first to LSB last)
3. write the flash

- flash is programmed 1 page at a time
- there are 64 words (@ 16 bits, or 2 bytes, each) to a page
- to load a word, target the word's address within the page (6 LSBs), so 00XXXXXX.
  then: 1st load the lower byte; and 2nd load the upper byte
- once the desired words within the page are loaded... write the page (wait at least like 10ms or something)

4.


read RDY/BSYb from PD1 on target to determine whether spi instruction has finished (probably don't need to worry about this)

*/

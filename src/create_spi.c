#include <stdio.h>
#include <target_bin.h>

// TODO:
// - include options for program placement boot section vs. regular section
// - include RDY/BSYb checks to operate at fastest possible speed
// - verify written memory with sent memory (corruption checks)
// - send these SPI instructions over USART so that the host
//     microcontroller cannot run out of memory

// create spi instructions to write program to target flash memory
int main() {
  unsigned char instructions[32768]; // what should max be?
  int i, ic = 0; // ic is instruction counter

  // enable programming
  unsigned char programming_enable[] = { 0xac, 0x53, 0x00, 0x00 };
  for(i = 0; i < 4; i++, ic++) {
    instructions[ic] = programming_enable[i];
  }

  // erase chip
  unsigned char chip_erase[] = { 0xac, 0x80, 0x00, 0x00 };
  for(i = 0; i < 4; i++, ic++) {
    instructions[ic] = chip_erase[i];
  }

  // target flash memory has size limit of 16K (8K*(2bytes/word))
  if(target_bin_len > 16384) {
    target_bin_len = 16384;
  }

  int j, page_num;
  for(j = 0; j < target_bin_len; j++) {
    page_num = j / 128;
    int word_num = (j / 2) % 64;

    // write the previous page when it fills up
    if(j && j % 128 == 0) {
      unsigned char write_program_memory_page[] = { 0x4c, page_num - 1, 0x20, 0x00 };
      for(i = 0; i < 4; i++, ic++) {
        instructions[ic] = write_program_memory_page[i];
      }
    }
    
    if(j & 1) { // load high byte to word address `word_num` in page
      unsigned char load_program_memory_page_high_byte[] = { 0x48, 0x00, word_num, target_bin[j] };
      for(i = 0; i < 4; i++, ic++) {
        instructions[ic] = load_program_memory_page_high_byte[i];
      }
    }
    else { // write low bite to word address `word_num` in page
      unsigned char load_program_memory_page_low_byte[] = { 0x40, 0x00, word_num, target_bin[j] };
      for(i = 0; i < 4; i++, ic++) {
        instructions[ic] = load_program_memory_page_low_byte[i];
      }
    }
  }

  // write the final page (we didn't get to write it in the loop)
  unsigned char write_program_memory_page[] = { 0x4c, page_num, 0x20, 0x00 };
  for(i = 0; i < 4; i++, ic++) {
    instructions[ic] = write_program_memory_page[i];
  }

  // all done! print hex
  for(i = 0; i < ic; i++) {
    if(i && !(i % 4)) {
      printf("\n");
    }
    printf("%02x ", instructions[i] & 0xff);
  }
  printf("\n");
  
  return 0;
}

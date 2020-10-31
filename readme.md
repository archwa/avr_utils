AVR Utilities
---

This repository contains utilities for AVR microcontrollers.

## What

- "Slow" SPI master controller programmer for extremely low frequency clock speeds at the target MCU

## Goals

For the initial programmer:
- Create a better USART to (slow) SPI master controller for initially programming target AVR MCU
  - ...currently the USART puts all the (slow) SPI master controller instructions (which contain the target MCU program) onto the master controller MCU memory, which then executes.
  - This can be a problem because the target MCU program, in combination with the SPI instructions that write it to the target MCU, can be too large for the master controller MCU memory.
  - I want to make it so that a USART PC-to-master controller connection directly interfaces with the SPI master controller, so that the master controller memory has no chance of filling up.
  - In this way, the master controller is just a USART-to-SPI translator for USART serial commands from the PC.
- Create a high-speed SPI master controller for initially programming target AVR MCU
  - Currently, I use a SLOW SPI master controller configuration that uses a custom SCK frequency.
  - I did this because I want the target MCU to run at a low frequency for now, and, from what I could tell, the low frequency at the target MCU is not compatible with the high frequency of the built-in SPI master controller frequency.
  - In future, I want to be able to use the high-speed built-in SPI controller of the master controller MCU, for when I might be using a high frequency at the target MCU.
  - (Using the slow SPI master controller would still work because the operating frequency is configurable - but it is using some other data ports not designated specifically for SPI communication on the master controller MCU.)
- Make it easier to select what programs to send over initially, besides just the bootloader or whatever


For software and subsequent programming options:
- Create a custom AVR bootloader that allows for in-system programming (ISP), probably over USART or similar (cuz I want to be able to just program my microcontroller using my PC's virtual serial port, not like the SPI stuff mentioned above - although I concede that this would make re-programming the bootloader slightly tricker since it would be over-writing itself... maybe a reserved spot in flash or EEPROM to load this? it's possible))
  - ...which just means the microcontroller can program itself/modify the flash/EEPROM memories while it is itself running.
  - The goal is to create a bootloader that allows the user to select from: (1) do ISP, (2) run the currently loaded program, (3) modify other stuff (fuse/lock bits, if possible??? and/or standard program size, program offsets, etc. so that multiple programs can be stored on the MCU)), (4) reset, and maybe a few other fun options for boot
- Maybe an operating system at some point? Lol not much of a point since there isn't much memory/peripherals... but I think it would be a fun adventure!

- Maybe some other boring stuff, idk

## Background / Storytime
In 2010, my grandfather gifted me a "NerdKit" (http://www.nerdkits.com/) for my birthday.
It contained, among other things, an ATmega168 microcontroller.
This kit was the primary inspiration for me to study the electrical engineering discipline.

Recently, I have taken an interest in programming it, but I do not own any specialized hardware programmers.
So, I began an investigation into the ATmega168 datasheets from Microchip (who bought Atmel) to see if there was some other way of programming it.
What I found was some information in section 28 "Memory Programming" that included details of the Serial Peripheral Interface (SPI).
When configured appropriately, the SPI allows for programming the on-board flash memory, EEPROM memory, fuse bits, and lock bits.
It also allows for a few other cool things, like reading memory, or reading the microcontroller signature bits.

But what I think is coolest about the SPI is that, in theory, no specialized hardware is required.
You just need maybe a couple of switches/buttons and a sufficient voltage difference.In this way, you can program the microcontroller purely by hand.
But, you probably don't want to do that.

So here's what I did:

First, I wanted to use an external clock ((1) 555 timer w/ potentiometer, or (2) manual button press) for the microcontroller, so I manually programmed the microcontroller fuse bits using the SPI and a couple of buttons hooked up to SCK and MOSI.
After doing this, the microcontroller uses the external clock supplied to XTAL1.
I also programmed the fuse bits so that the clock in use would be output to CLKO.
This way, I could monitor whether the microcontroller was actually using the external clock.

Once I verified that the microcontroller was using the external clock, I sought to create an SPI master controller using a spare Arduino Nano v3.0 I had lying around.
This felt kind of icky, since I wanted to do as much from scratch as possible, but I think it's fine since it's a one time thing. The goal is to program a bootloader once, and then use ISP for each time I want to subsequently program the target MCU.

Yeah I think that's it. You can read more in the "Goals" section.

---

Yes, I know this is a sloppy initial readme - sue me! It'll develop over time.

### Overview : 
Hex file is text based format used to represent a binary data in way that's easly readable and transferable it is used to program the microcontrollers and microprocessors.
### Intel Hex types : 
- INHX8M which targets 8-bit microcontrollers like (MICROCHIP).
- INHX8S which targest 8-bit microcontrollers but with separate memories.
- INHX32 which targets the 32-bit microcontrollers like (ARM, PIC32).

Our goal is to read a hex file of type INHX8M and generate the assembly code back.
### INHX8M Format : 
:[BYTE COUNT][ADDRESS][RECORD TYPE][DATA SECTION][CHECK SUM]


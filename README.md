# INHX8M Disassembler

## 📜 Description : 

This project is a disassembler for **INHX8M** hex files, a format commonly used to represent firmware for 8-bit microcontrollers such as those from **Microchip (PIC)**. The tool reads a hex file in Intel Hex format and converts the machine code back into human-readable **assembly language** instructions.

This is useful for:
- Reverse engineering firmware
- Debugging embedded systems

## 🎯 Objective : 

To parse and disassemble an **INHX8M HEX file** and reconstruct the original **assembly code** using the instruction set of the target microcontroller.

---

## 📁 File Format: INHX8M

Each line in the hex file follows this format:
: [BYTE COUNT] [ADDRESS] [RECORD TYPE] [DATA] [CHECKSUM]


### Example:
:10010000214601360121470136007EFE09D2190140

- `: ` → Start of record  
- `10` → Byte count (16 bytes)  
- `0100` → Address (0x0100)  
- `00` → Record type (00 = data)  
- `2146...` → Data bytes  
- `40` → Checksum  

## 🧰 Requirements : 
As you see there is a hex file named as source.HEX that will be used to read from the records, so the file must exist where the source file is (source.c).

## How to Run : 
Upload the repository and then open the command line and execute the bash script.

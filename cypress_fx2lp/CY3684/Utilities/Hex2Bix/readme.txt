This directory contains the Hex To Bix converter program
for the Anchor Chips EZ-USB chip.

This file converts the hex output file to a bix file that can be
downloaded directly into target memory. It also converts the hex
output to a file that can be directly loaded into an EEPROM.
It basically creates a memory image (including zeros) of the memory
space defined by the hex file.

Program Options:
HEX2BIX [-AIBRH?] [-S symbol] [-M memsize] [-O filename] Source
    Source - Input filename
    A      - Output file in the A51 file format
    B      - Output file in the BIX file format (Default)
    H|?    - Display this help screen
    I      - Output file in the IIC file format
    M      - Maximum memory limit (Default = 8k)
    O      - Output filename
    R      - Append bootload block to release reset
    S      - Public symbol name for linking



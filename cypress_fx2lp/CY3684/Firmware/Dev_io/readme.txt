This directory contains the dev_io 8051 firmware 

The purpose of this software is to demonstrate how
to use the buttons and LED on the EZ-USB developer's
kit.

The device I/O example program uses the LED on the Development board
to display a running application program on the EZ-USB chip.
The buttons on the development board may be pressed to count up (F3),
count down (F2), and reset the seven segment LED to "0" (F1) or "F" (F4).
The code is written in C and uses several EZ-USB library functions.  
It does not use the FrameWorks. 
The dev_io.hex file loads into internal memory.

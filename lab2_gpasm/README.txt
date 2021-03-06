
The files in this directory are direct descendents of the "Lab 2
peripheral firmware for PIC18F2455" files published by Brad Minch at:

http://pe.ece.olin.edu/ece/projects.html


I have made minor changes to his code with these two goals in mind:

1. So the project would assemble under linux using gpasm, and
2. So the project would be bootloadable on a full speed picdem board
without requiring changes to the bootloader.


Making the assembly code work with gpasm mostly required modifying the
macro syntax in the file ENGR2210.inc, because gpasm's macro processor
doesn't seem to work exactly as mpasm's does.  I'm indebted to Bill
Freeman for showing me how to do this.

Making it work with the PICDEM FS mainly meant (a) understanding the
oscillator settings and (b) making sure the code went in the right
part of program memory.  With the picdem's 20 MHz crystal, and not
changing its default configuration bit settings, this project needs to
be run as a full speed usb device.  (This is best explained in section
2.3 of the pdf file for the PIC18F2455 family "Oscillator settings for
USB").

To use these files, just type "make" on a system that has gpasm
installed (my system has gpasm-0.13.1 beta).  This should produce an
asm output file.  I use Alessandro Zummo's perl script "picdem2.pl" to
load the file into the board, and I use the simple script called "r"
to call that script.  (You'll find picdem2.pl in the archives of the
GNUPIC list.)



I chose to use Brad Minch's code because of the relative clarity of the
lab2.asm file.  In hindsight I have done very little to Brad Minch's
code here, and some of what I've had to do make its lose some of its
clarity (in the ENGR2210.inc file).  But I'm making what I did
available because it may help others like me, which is the least I can
do considering all th ehelp others have given to me!

-Ben Dugan

September 14, 2005

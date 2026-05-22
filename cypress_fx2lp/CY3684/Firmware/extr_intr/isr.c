#include <stdio.h>
#include "fx2.h"
#include "fx2regs.h"
xdata volatile unsigned char D2ON         _at_ 0x8800;
xdata volatile unsigned char D2OFF        _at_ 0x8000;
xdata volatile unsigned char D3ON         _at_ 0x9800;
xdata volatile unsigned char D3OFF        _at_ 0x9000;
xdata volatile unsigned char D4ON         _at_ 0xA800;
xdata volatile unsigned char D4OFF        _at_ 0xA000;
xdata volatile unsigned char D5ON         _at_ 0xB800;
xdata volatile unsigned char D5OFF        _at_ 0xB000;

 unsigned char dum;
 static int w;
 static int x;
 static int y;
 static int z;
//interrupt 0 ,interrupt 1 , interrupt 10, interrupt 11 , interrupt 12 tells the compiler to look for the ISR at address 0x0003,0x0013,
//0x0053 ,0x005B,0x0063 respectively . The numbers 10,11 ,12 are derived from the natural priority of the interrupt for INT4/5/6 .
//Numbers 0 and 1 are (natural priority -1).


void ISR_EXTR0(void) interrupt 0                           
{ 
  TCON &= 0xFD;         // Clear INT0  TCON.1 Flag . This is also automatically cleared by hardware.
  IOC ^= 0x01;		    // Toggle pin 0 of PortC

  dum=D2ON;
  w^=1;
  if (w)
  {dum=D2OFF;}
}
void ISR_EXTR1(void) interrupt 2
{ 
  TCON &= 0xF7;         // Clear INT1  TCON.3 Flag . This is also automatically cleared by hardware. 
  IOC ^= 0x02;		    // Toggle pin 1 of PortC
  dum=D3ON;
  x^=1;
  if (x)
  {dum=D3OFF;}
 }
void ISR_EXTR4(void) interrupt 10
{
  	EXIF &= 0xBF;      // Clear INT4 EXIF.6  Flag
	IOC ^= 0x10;	   // Toggle pin 4 of PortC
	  dum=D4ON;
  y^=1;
  if (y)
  {dum=D4OFF;}
}
void ISR_EXTR5(void) interrupt 11
{
    EXIF &= 0x7F;      // Clear INT5 EXIF.7  Flag
	IOC ^= 0x20;	   // Toggle pin 5 of PortC
	  dum=D5ON;
  z^=1;
  if (z)
  {dum=D5OFF;}
}
void ISR_EXTR6(void) interrupt 12
{
   EICON &= 0xF7;      // Clear INT6 EICON.3  Flag
   IOC  ^= 0x40;	   // Toggle pin 6  of PortC
}

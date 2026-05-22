// RAM test - avoids the range from 0x4000 to 0x5100 since it is loaded there
#define ALLOCATE_EXTERN // Allocate explicit register addresses here
#include "Fx2.h"
#include "Fx2regs.h"

#define START_MEM00		0x0C00
#define LENGTH_MEM00	(0x2000-START_MEM00)
#define START_MEM01		0x2000
#define LENGTH_MEM01	0x2000 // avoids the range from 0x4000 to 0x5100
							   // this program code is loaded at 0x4000-0x5000
							   // and XDATA is located at 0x5000-0x5100
#define START_MEM02		0x5100 // check 0x5100 to 0x8000
#define LENGTH_MEM02	(0x8000-START_MEM02)

#define START_MEM03		0xE000
#define LENGTH_MEM03	0x200  // scratch RAM

#define START_MEM04	  0xE400  // check GPIF waveform buffer
#define LENGTH_MEM04  0x007F  // and stop at 384 byte(s) reserved

#define START_MEM05   0xE740  // check endpoint 0 & 1 buffer(s)
#define LENGTH_MEM05  0x00BF  // and stop at 2k Reserved & 2,4,6,8 buffer(s)
                              
#define START_MEM06   0xF000  // check EP2
#define LENGTH_MEM06  0x0200  

#define START_MEM07   0xF400  // check EP4
#define LENGTH_MEM07  0x0200  

#define START_MEM08   0xF800  // check EP6
#define LENGTH_MEM08  0x0200  

#define START_MEM09   0xFC00  // check EP8
#define LENGTH_MEM09  0x0200  

#define LED_ADDR		0x21

BYTE xdata Digit[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x98, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e };
BYTE xdata Alpha[] = { 0x88,0x83,0xa7,0xa1,0x86,0x8e,0x90,0x8b,0xef,0xe1,0x87,0xc7,0x80,0xab,0xa3,0x8c,0x40,0xaf,0x92,0x87,0xc1,0xcd,0x80,0x89,0x99,0xa4 };
BYTE xdata blank = 0xff;

BYTE err_lvl;

extern BOOL test_data(void);

long test_mem(BYTE xdata *addr, WORD length)
{
	WORD i;
	BYTE data1;
	BYTE data2;

  err_lvl = 1;

	for(i = 0; i < length; ++i)
		addr[i] = 0x00;
	for(i = 0; i < length; ++i)
		if(addr[i] != 0x00)
			return(addr+i);

  err_lvl = 2;

	for(i = 0; i < length; ++i)
		addr[i] = 0xff;
	for(i = 0; i < length; ++i)
		if(addr[i] != 0xff)
			return(addr+i);

  err_lvl = 3;

	for(i = 0; i < length; ++i)
		addr[i] = 0xaa;
	for(i = 0; i < length; ++i)
		if(addr[i] != 0xaa)
			return(addr+i);

  err_lvl = 4;

	for(i = 0; i < length; ++i)
		addr[i] = 0x55;
	for(i = 0; i < length; ++i)
		if(addr[i] != 0x55)
			return(addr+i);

  err_lvl = 5;

	for(i = length-1; i > 0; --i) // Check the address bus -- LSB only
		addr[i] = LSB(i) + MSB(i) * 3;
	for(i = length-1; i > 0; --i)
		if(addr[i] != (BYTE) (LSB(i) + MSB(i) * 3)) 
			return(addr+i);

  err_lvl = 6;

  for(i = 0; i < length; ++i)
		addr[i] = LSB(i) + MSB(i) * 3;
	for(i = 0; i < length; ++i)
		if(addr[i] != (BYTE) (LSB(i) + MSB(i) * 3))
			return(addr+i);

  err_lvl = 7;

	if( ((addr+length) <= 0x2000) ||((addr+length) >= 0xF000) )  // internal data access only via AUTODAT1
	{
		for(i = 0; i < length; ++i)
			addr[i] = 0x00;
		APTR1H = MSB(addr);
		APTR1L = LSB(addr);
  		for(i = 0; i < length; ++i)
		{
			data1 = LSB(i) + MSB(i) * 3;
			EXTAUTODAT1 = data1;
		}

		APTR1H = MSB(addr);
		APTR1L = LSB(addr);
		for(i = 0; i < length; ++i)
		{
			data2 = EXTAUTODAT1;
			if(data2 != (BYTE) (LSB(i) + MSB(i) * 3))
				return(addr+i);
		}
	}

	return(-1);
}

void status(WORD stat)
{
	WORD saveStat = stat;
	BYTE i;

	while(TRUE)
	{
		stat=saveStat;
		for (i = 0; i < 4; i++, stat <<= 4)
		{
			EZUSB_WriteI2C(LED_ADDR, 0x01, &(Digit[(stat>>12)&0xf]));
			EZUSB_WaitForEEPROMWrite(LED_ADDR);
			EZUSB_Delay(400);
			EZUSB_WriteI2C(LED_ADDR, 0x01, &blank);
			EZUSB_WaitForEEPROMWrite(LED_ADDR);
			EZUSB_Delay(200);
		}
		EZUSB_WriteI2C(LED_ADDR, 0x01, &blank);
		EZUSB_WaitForEEPROMWrite(LED_ADDR);
		EZUSB_Delay(500);
	}
}

void statString(char *stat)
{
	BYTE i;

		for (i = 0; stat[i]; i++)
		{
			EZUSB_WriteI2C(LED_ADDR, 0x01, &(Alpha[(stat[i] & 0x5f)-'A']));
			EZUSB_WaitForEEPROMWrite(LED_ADDR);
			EZUSB_Delay(400);
			EZUSB_WriteI2C(LED_ADDR, 0x01, &blank);
			EZUSB_WaitForEEPROMWrite(LED_ADDR);
			EZUSB_Delay(200);
		}
		EZUSB_WriteI2C(LED_ADDR, 0x01, &blank);
		EZUSB_WaitForEEPROMWrite(LED_ADDR);
		EZUSB_Delay(500);
}

main()
{
	long addr;
	BYTE xdata	buttons = 0;
	BYTE xdata	cont_loop = 1;


	EZUSB_InitI2C();
  CPUCS = 0x10;         // set 48Mhz bit 
  IFCONFIG = 0xF6;	  // GPIF Int clock 48Mhz GSTATE SYNC  1111 0110
    
  // enable dual autopointer feature
  AUTOPTRSETUP |= 0x01;

	EZUSB_WriteI2C(LED_ADDR, 0x01, &(Digit[0]));
	EZUSB_WaitForEEPROMWrite(LED_ADDR);

	while(cont_loop)
	{
  
		addr = test_mem(START_MEM00,LENGTH_MEM00);
		if (addr != -1)
            status(addr);

		// test to see if this is an FX2 or an FX2LP.  Do this by writing to the
		// ECCCFG register which only exists in FX2LP
		ECCCFG = 0x01;
		if (ECCCFG == 0x01)
		{
			addr = test_mem(START_MEM01,LENGTH_MEM01);
			if (addr != -1)
	            status(addr);
		}

//		addr = test_mem(START_MEM02,LENGTH_MEM02);
//		if (addr != -1)
//            status(addr);

		addr = test_mem(START_MEM03,LENGTH_MEM03);
		if (addr != -1)
            status(addr);

		addr = test_mem(START_MEM06,LENGTH_MEM06);
		if (addr != -1)
            status(addr);

		addr = test_mem(START_MEM07,LENGTH_MEM07);
		if (addr != -1)
            status(addr);

		addr = test_mem(START_MEM08,LENGTH_MEM08);
		if (addr != -1)
            status(addr);

		addr = test_mem(START_MEM09,LENGTH_MEM09);
		if (addr != -1)
            status(addr);

		IFCONFIG = 0x02;

		addr = test_mem(START_MEM04,LENGTH_MEM04); // Do WF Mem test
		if (addr != -1)
            status(addr);

		addr = test_mem(START_MEM05,LENGTH_MEM05);
		if (addr != -1)
            status(addr);

//		xd = (BYTE xdata *)START_MEM00;
//		cd = (BYTE code *)START_MEM01;

//		*xd = *cd + 1;

//		if(*xd != *cd)
		{
            statString("good");
		}
	}

	statString("dog");					// Never happen
	EZUSB_ReadI2C(0x20,0x01,&buttons);	// Read button states

}


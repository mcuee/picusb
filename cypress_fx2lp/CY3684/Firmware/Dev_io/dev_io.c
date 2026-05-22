//-----------------------------------------------------------------------------
//   File:      devio.c
//   Contents:  main module of a simple non-renumerating firmware example
//
// The purpose of this software is to demonstrate how
// to use the buttons and LED on the EZ-USB developer's
// kit.
// 
// The device I/O example program uses the LED on the Development board
// to display a running application program on the EZ-USB chip.
// The buttons on the development board may be pressed to count up (F3),
// count down (F2), and reset the seven segment LED to "0" (F1) or "F" (F4).
// The code is written in C and uses several EZ-USB library functions.  
// It does not use the FrameWorks. 
//
// $Archive: /USB/Examples/FX2LP/dev_io/dev_io.c $
// $Date: 6/22/06 2:55p $
// $Revision: 5 $
//
//
//-----------------------------------------------------------------------------
// Copyright 2003, Cypress Semiconductor Corporation
//-----------------------------------------------------------------------------
#define ALLOCATE_EXTERN

#include "fx2.h"
#include "fx2regs.h"

#define PF_IDLE			0
#define PF_GETKEYS		1

#define KEY_WAKEUP		0
#define KEY_F1			1
#define KEY_F2			2
#define KEY_F3			3

#define BTN_ADDR		0x20
#define LED_ADDR		0x21

BYTE xdata Digit[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x98, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e };

BYTE I2C_ReadByte(BYTE I2CAddr);
void I2C_WriteByte(BYTE I2CAddr, BYTE dat);

main()
{
	BYTE	num = 0;
	BYTE xdata	buttons;
	BYTE	kstates = 0xff;
	BYTE	kdeltas;
	BYTE	key;
	BYTE	display = TRUE;
    bit     oldEA;


	while(TRUE)
	{
        // Read the I2C buttons
        // Disable interrupts around I2C access to avoid conflict with the I2C based
        // debug monitor used by FX2LP18
        oldEA = EA;
        EA = 0;
        buttons = I2C_ReadByte(BTN_ADDR);
        EA = oldEA;;

		kdeltas = kstates ^ buttons;			// 
		kstates = buttons;
		key = 0;

		while(kdeltas)
		{
			if(kdeltas & 0x01)
			{
				if(!((kstates >> key) & 0x01))
					switch(key)
					{
						case KEY_F1:
							if(--num > 0x0f)
								num = 0x0f;
							break;
						case KEY_F2:
							if(++num > 0x0f)
								num = 0;
							break;
						case KEY_WAKEUP:
							num = 0;
							break;
						case KEY_F3:
							num = 0x0f;
					}
				display = TRUE;
			}
			kdeltas = kdeltas >> 1;
			++key;
		}
		if(display)
		{
            // Write to the I2C 7-segment display
            // Disable interrupts around I2C access to avoid conflict with the I2C based
            // debug monitor used by FX2LP18
            oldEA = EA;
            EA = 0;
            I2C_WriteByte(LED_ADDR, Digit[num]);
            EA = oldEA;;

			display = FALSE;
		}
	}
}

void I2C_WriteByte(BYTE I2CAddr, BYTE dat)
{
    // wait for STOP
    while (I2CS & bmSTOP);

    // set the START bit and write the address
    I2CS = bmSTART;
    I2DAT = I2CAddr << 1;

    // wait for done
    while (!(I2CS & bmDONE));
    
    // write the requested data
    I2DAT = dat;

    // wait for done
    while (!(I2CS & bmDONE));

    // set the STOP bit    
    I2CS = bmSTOP;
}

BYTE I2C_ReadByte(BYTE I2CAddr)
{
    BYTE retval;

    // wait for STOP
    while (I2CS & bmSTOP);

    // set the START bit and write the address
    I2CS = bmSTART;
    I2DAT = (I2CAddr << 1) | 0x01;

    // wait for done
    while (!(I2CS & bmDONE));

    // check for ACK, return 0 if no response
    if (!(I2CS & bmACK))
    {
        I2CS = bmSTOP;
        retval = 0;
    }
    else
    {
        // read the dummy byte
        retval = I2DAT;
        I2CS = bmLASTRD;

        while (!(I2CS & bmDONE));
        I2CS = bmSTOP;

        retval = I2DAT;
    }

    return(retval);
}


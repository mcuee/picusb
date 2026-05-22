//--------------------------------------------------------------------------
//
// Copyright 2008, Cypress Semiconductor Corporation.
//
// This software is owned by Cypress Semiconductor Corporation (Cypress)
// and is protected by and subject to worldwide patent protection (United
// States and foreign), United States copyright laws and international
// treaty provisions. Cypress hereby grants to licensee a personal,
// non-exclusive, non-transferable license to copy, use, modify, create
// derivative works of, and compile the Cypress Source Code and derivative
// works for the sole purpose of creating custom software in support of
// licensee product to be used only in conjunction with a Cypress integrated
// circuit as specified in the applicable agreement. Any reproduction,
// modification, translation, compilation, or representation of this
// software except as specified above is prohibited without the express
// written permission of Cypress.
//
// Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND,EXPRESS OR IMPLIED,
// WITH REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Cypress reserves the right to make changes without further notice to the
// materials described herein. Cypress does not assume any liability arising
// out of the application or use of any product or circuit described herein.
// Cypress does not authorize its products for use as critical components in
// life-support systems where a malfunction or failure may reasonably be
// expected to result in significant injury to the user. The inclusion of
// Cypress' product in a life-support systems application implies that the
// manufacturer assumes all risk of such use and in doing so indemnifies
// Cypress against all charges.
//
// Use may be limited by and subject to the applicable Cypress software
// license agreement.
//
//--------------------------------------------------------------------------
//*****************************************************************************
//*****************************************************************************
//  FILENAME: extr_intr.c
//   Version: 1.0, Updated on 04 Jan 2010
//
//  DESCRIPTION: External Interrupt in FX1/FX2LP.
//
//-----------------------------------------------------------------------------
//  Copyright (c) Cypress MicroSystems 2000-2003. All Rights Reserved.
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************


//Project Objective:
//	The objective of this project is to demonstrate the usage of External interrupts in FX2LP, by providing frameworks based 
//	External interrupt program written in C. 

//Overview:
//	The FX2LP has 5 external interrupts: IE0, IE1, IE4, IE5, and IE6. IE0, IE1 are either Edge triggered or level triggered and 
//	IE4, IE5, IE6 are edge triggered interrupts. In the project, Port C has been configured as an output port. When an INT0 interrupt occurs, 
//	PC.0 is toggled. Similarly, on INT1 /INT4/INT5/INT6, PC.1 /PC.4/PC.5/PC.6 are toggled. The code for toggling the Port C pins is written 
//	inside for the Interrupt Service Routine [ISR] for these interrupts, in the isr.c file.
//
//Operations:
//	Refer to the document External_Interrupt.doc
//
//-------------------------------------------------------------------------------------------
// Code below
//-------------------------------------------------------------------------------------------

// Do not generate interrupt vectors
#pragma NOIV               
//-----------------------------------------------------------------------------
//   File:       Fx2_extr_intr.c
//   Contents:   Hooks required to implement USB peripheral function.
//   Copyright (c) 2000 Cypress Semiconductor All rights reserved
//-----------------------------------------------------------------------------
#include "fx2.h"
#include "fx2regs.h"
#include "syncdly.h"            // SYNCDELAY macro

extern BOOL GotSUD;             // Received setup data flag
extern BOOL Sleep;
extern BOOL Rwuen;
extern BOOL Selfpwr;

BYTE Configuration;             // Current configuration
BYTE AlternateSetting;          // Alternate settings
BYTE IbnFlag;

#define VR_NAKALL_ON    0xD0
#define VR_NAKALL_OFF   0xD1
#define VR_IMT4_TRIGGER   0xD2

//-----------------------------------------------------------------------------
// Task Dispatcher hooks
// The following hooks are called by the task dispatcher.
//-----------------------------------------------------------------------------

void TD_Init(void)             // Called once at startup              
{
   CPUCS = ((CPUCS & ~bmCLKSPD) | bmCLKSPD1) ;

   // set the slave FIFO interface to 48MHz
   IFCONFIG |= 0x40;
  
 

  // Registers which require a synchronization delay, see section 15.14
  // FIFORESET        FIFOPINPOLAR
  // INPKTEND         OUTPKTEND
  // EPxBCH:L         REVCTL
  // GPIFTCB3         GPIFTCB2
  // GPIFTCB1         GPIFTCB0
  // EPxFIFOPFH:L     EPxAUTOINLENH:L
  // EPxFIFOCFG       EPxGPIFFLGSEL
  // PINFLAGSxx       EPxFIFOIRQ
  // EPxFIFOIE        GPIFIRQ
  // GPIFIE           GPIFADRH:L
  // UDMACRCH:L       EPxGPIFTRIG
  // GPIFTRIG
  
  // Note: The pre-REVE EPxGPIFTCH/L register are affected, as well...
  //      ...these have been replaced by GPIFTC[B3:B0] registers

  // default: all endpoints have their VALID bit set
  // default: TYPE1 = 1 and TYPE0 = 0 --> BULK  
  // default: EP2 and EP4 DIR bits are 0 (OUT direction)
  // default: EP6 and EP8 DIR bits are 1 (IN direction)
  // default: EP2, EP4, EP6, and EP8 are double buffered

  // we are just using the default values, yes this is not necessary...
  EP1OUTCFG = 0xA0;
  EP1INCFG = 0xA0;
  SYNCDELAY;                    // see TRM section 15.14
  EP2CFG = 0xA2;
  SYNCDELAY;                    
  EP4CFG = 0xA0;
  SYNCDELAY;                    
  EP6CFG = 0xE2;
  SYNCDELAY;                    
  EP8CFG = 0xE0;

  // out endpoints do not come up armed.  Arm EP1, EP2 and EP4 OUT  endpoints
   EP1OUTBC = 0x40;				// arm the EP1 OUT endpoint by writing to the byte count
  // since the defaults are double buffered we must write dummy byte counts twice
  SYNCDELAY;                    
  EP2BCL = 0x80;                // arm EP2OUT by writing byte count w/skip.
  SYNCDELAY;                    
  EP2BCL = 0x80;
  SYNCDELAY;                    
  EP4BCL = 0x80;                // arm EP4OUT by writing byte count w/skip.
  SYNCDELAY;                    
  EP4BCL = 0x80;    

 
  
  
 /************************************Enable Endpoint Interrupt for EP1IN and EP1OUT*******************************************/

		EPIE |= bmBIT3 ; // Enable EP1 OUT Endpoint interrupts
		EPIE |= bmBIT2; // Enable  EP1 IN  Endpoint interrupts
  
  // enable dual autopointer feature
  AUTOPTRSETUP |= 0x01;

  Rwuen = TRUE;                 // Enable remote-wakeup
								// Configure  for  external interrupts

/************************************************************Port C configuration***************************/

	PORTCCFG = 0x00;            // PORTC is is configured as an I/O, alternately it can output the lower address of enabled GPIF address pins
	OEC  = 0xFF;                // PORTC is an output
	IOC = 0xFF;                 // Initialize PORTC to all LOW

/************************************************************External Interrupt***************************/

//INT0# and INT1#

	PORTACFG = 0x03;            // PA0 and PA1 are pins for INT0 and INT1 respectively.
	TCON |= 0x05;               // INT0 and INT1 are configured as Edge triggered interrupts. 

//INT4

	INTSETUP &= ~0x02;          // If INTSETUP.1=0,then INT4 is supplied by the pin.Else, the interrupt is supplied internally FIFO/GPIF sources.

//INT5#  is a dedicated  pin , available in the 100 amd 128 pin packages.

//INT6#	

	PORTECFG = 0x20;            // PE5  is  INT6 
   	OEE &= ~0x20;
	
//Enable  External Interrupts
	EIE |= 0x1C;		        // Enable External Interrupts 4, 5 and 6
    IE  |= 0x05;			    // Enable External Interrupts 0 and 1	
	
//Clear  Flags
    EXIF &= 0xBF;      			// Clear INT4 EXIF.6  Flag
	EXIF &= 0x7F;      			// Clear INT5 EXIF.7  Flag
	EICON &= 0xF7;      		// Clear INT6 EICON.3  Flag
 	EA   = 1;				    // Enable Global Interrupt

   /*********************************************Enable the IBN interrupt for EP2 *************************************************************/

    // NAKIRQ = bmBIT0;              // clear the global IBN IRQ
  	 NAKIE |= bmBIT0;              // enable the global IBN IRQ

 // IbnFlag = 0x00;               // clear our IBN flag
  //IBNIRQ = 0xFF;                // clear any pending IBN IRQ
  IBNIE |= bmEP6IBN ; // enable the IBN interrupt for EP6  
  NAKIRQ |= ~bmIBN;                   // clear any pending PING-NAK IRQ

   /********************Enable the PING-NAK interrupt for EP4***********************************************************************************/

  NAKIE |=  bmEP4PING;     // enable the PING-NAK interrupt for  EP4
}


void TD_Poll(void)              // Called repeatedly while the device is idle
{
   
  WORD i;
  WORD count;
    INTSETUP &= ~bmAV4EN;     // Disable INT4 autovectoring so that we use  the external INT4
	 

if(!(EP2468STAT & bmEP4EMPTY))
  { // check EP4 EMPTY(busy) bit in EP2468STAT (SFR), core set's this bit when FIFO is empty
     if(!(EP2468STAT & bmEP8FULL))
     {  // check EP8 FULL(busy) bit in EP2468STAT (SFR), core set's this bit when FIFO is full
        APTR1H = MSB( &EP4FIFOBUF );
        APTR1L = LSB( &EP4FIFOBUF );

        AUTOPTRH2 = MSB( &EP8FIFOBUF );
        AUTOPTRL2 = LSB( &EP8FIFOBUF );

        count = (EP4BCH << 8) + EP4BCL;
        //EXTAUTODAT2 = EXTAUTODAT1+1;
        // loop EP4OUT buffer data to EP8IN
        for( i = 0x0000; i < count; i++ )
        {
           // setup to transfer EP4OUT buffer to EP8IN buffer using AUTOPOINTER(s)
           EXTAUTODAT2 = EXTAUTODAT1;
        }
        
        EP8BCH = EP4BCH;  
        SYNCDELAY;  
        EP8BCL = EP4BCL;        // arm EP8IN
        SYNCDELAY;                    
        EP4BCL = 0x80;          // re(arm) EP4OUT
     }
  }
}

BOOL TD_Suspend(void)          // Called before the device goes into suspend mode
{
   return(TRUE);
}

BOOL TD_Resume(void)          // Called after the device resumes
{
   return(TRUE);
}

//-----------------------------------------------------------------------------
// Device Request hooks
//   The following hooks are called by the end point 0 device request parser.
//-----------------------------------------------------------------------------

BOOL DR_GetDescriptor(void)
{
   return(TRUE);
}

BOOL DR_SetConfiguration(void)   // Called when a Set Configuration command is received
{
   Configuration = SETUPDAT[2];
   return(TRUE);            // Handled by user code
}

BOOL DR_GetConfiguration(void)   // Called when a Get Configuration command is received
{
   EP0BUF[0] = Configuration;
   EP0BCH = 0;
   EP0BCL = 1;
   return(TRUE);            // Handled by user code
}

BOOL DR_SetInterface(void)       // Called when a Set Interface command is received
{
   AlternateSetting = SETUPDAT[2];
   return(TRUE);            // Handled by user code
}

BOOL DR_GetInterface(void)       // Called when a Set Interface command is received
{
   EP0BUF[0] = AlternateSetting;
   EP0BCH = 0;
   EP0BCL = 1;
   return(TRUE);            // Handled by user code
}

BOOL DR_GetStatus(void)
{
   return(TRUE);
}

BOOL DR_ClearFeature(void)
{
   return(TRUE);
}

BOOL DR_SetFeature(void)
{
   return(TRUE);
}

BOOL DR_VendorCmnd(void)
{
  BYTE tmp;
  
  switch (SETUPDAT[1])
  {
     case VR_NAKALL_ON:
        tmp = FIFORESET;
        tmp |= bmNAKALL;      
        SYNCDELAY;                    
        FIFORESET = tmp;
        break;
	case VR_IMT4_TRIGGER:
			*EP0BUF = VR_IMT4_TRIGGER; // Fail
			EP0BCH = 0;
			EP0BCL = 1; // Arm endpoint with # bytes to transfer
			EXIF |= 0x40;  //Trigger  external  interrupt in SW
			break;
     case VR_NAKALL_OFF:
        tmp = FIFORESET;
        tmp &= ~bmNAKALL;      
        SYNCDELAY;                    
        FIFORESET = tmp;
        break;
     default:
        return(TRUE);
  }

  return(FALSE);
}

//-----------------------------------------------------------------------------
// USB Interrupt Handlers
//   The following functions are called by the USB interrupt jump table.
//-----------------------------------------------------------------------------

// Setup Data Available Interrupt Handler
void ISR_Sudav(void) interrupt 0
{
   GotSUD = TRUE;            // Set flag
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUDAV;         // Clear SUDAV IRQ
}

// Setup Token Interrupt Handler
void ISR_Sutok(void) interrupt 0
{
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUTOK;         // Clear SUTOK IRQ
}

void ISR_Sof(void) interrupt 0
{
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSOF;            // Clear SOF IRQ
}

void ISR_Ures(void) interrupt 0
{
   // whenever we get a USB reset, we should revert to full speed mode
   pConfigDscr = pFullSpeedConfigDscr;
   ((CONFIGDSCR xdata *) pConfigDscr)->type = CONFIG_DSCR;
   pOtherConfigDscr = pHighSpeedConfigDscr;
   ((CONFIGDSCR xdata *) pOtherConfigDscr)->type = OTHERSPEED_DSCR;

   EZUSB_IRQ_CLEAR();
   USBIRQ = bmURES;         // Clear URES IRQ
}

void ISR_Susp(void) interrupt 0
{
   Sleep = TRUE;
   EZUSB_IRQ_CLEAR();
   USBIRQ = bmSUSP;
}

void ISR_Highspeed(void) interrupt 0
{
   if (EZUSB_HIGHSPEED())
   {
      pConfigDscr = pHighSpeedConfigDscr;
      ((CONFIGDSCR xdata *) pConfigDscr)->type = CONFIG_DSCR;
      pOtherConfigDscr = pFullSpeedConfigDscr;
      ((CONFIGDSCR xdata *) pOtherConfigDscr)->type = OTHERSPEED_DSCR;
   }

   EZUSB_IRQ_CLEAR();
   USBIRQ = bmHSGRANT;
}
void ISR_Ep0ack(void) interrupt 0
{
}
void ISR_Stub(void) interrupt 0
{
}
void ISR_Ep0in(void) interrupt 0
{
}
void ISR_Ep0out(void) interrupt 0
{
}
void ISR_Ep1in(void) interrupt 0
{
	EP1OUTBC = 64;

	EZUSB_IRQ_CLEAR();//Clears the USB interrupt
	EPIRQ = bmBIT2;//Clears EP1 IN interrupt request 
}
void ISR_Ep1out(void) interrupt 0
{

	BYTE count;
	BYTE i;
	
	count = EP1OUTBC;// The count value is loaded from the byte count register
	if(!(EP1INCS & bmBIT1))
	{// Checks EP1IN availability
		for (i=0;i<count; i++)
		{
			EP1INBUF[i]=EP1OUTBUF[i];
		}
        EP1INBUF[0]=EP1OUTBUF[i]+1;
		EP1INBC =count;
	}
   
	EZUSB_IRQ_CLEAR();//Clears the USB interrupt
	EPIRQ = bmBIT3;//Clears EP1 OUT interrupt request  
}
void ISR_Ep2inout(void) interrupt 0
{
}
void ISR_Ep4inout(void) interrupt 0
{
}
void ISR_Ep6inout(void) interrupt 0
{
}
void ISR_Ep8inout(void) interrupt 0
{
}
void ISR_Ibn(void) interrupt 0
{
   int i;
   int byteCount;
   // disable IBN for all endpoints
  // IBNIE = 0x00;
   // clear the global USB IRQ
   EZUSB_IRQ_CLEAR();         

   // Find the EP with its IBN bit set
  /* for (i=0;i<8;i++)
   {
      if (IBNIRQ & (1 << i))
      {
         IbnFlag |= (1 << i);       // set the appropriate IBN flag bit
         IBNIRQ |= (1 << i);        // clear the IBN IRQ for this endpoint
      }
   }*/

 
  // if there is new data in EP2FIFOBUF and the IBN flag for EP6 has been set, then copy
  // the data from EP2 to EP6
  if(!(EP2468STAT & bmEP2EMPTY) )//&& (IbnFlag & bmEP6IBN) 
  {
     APTR1H = MSB( &EP2FIFOBUF );
     APTR1L = LSB( &EP2FIFOBUF );

     AUTOPTRH2 = MSB( &EP6FIFOBUF );
     AUTOPTRL2 = LSB( &EP6FIFOBUF );

     byteCount = (EP2BCH << 8) + EP2BCL;
    
     EXTAUTODAT2 = EXTAUTODAT1+1;

     for( i = 0x0001; i < byteCount; i++ )
     {
        EXTAUTODAT2 = EXTAUTODAT1;
     }

     EP6BCH = EP2BCH;  
     SYNCDELAY;  
     EP6BCL = EP2BCL;           // arm EP6IN

     //IbnFlag &= ~bmEP6IBN;      // clear the IBN flag
     IBNIRQ = bmEP6IBN;         // clear the IBN IRQ
     IBNIE |= bmEP6IBN;         // enable the IBN IRQ

     SYNCDELAY;                        
     EP2BCL = 0x80;             // re(arm) EP2OUT
  }

  // if there is new data in EP4FIFOBUF and the IBN flag for EP8 has been set, then copy
  // the data from EP2 to EP6
  
  NAKIRQ |= bmBIT0;          // clear the global IBN IRQ

   // re-enable IBN interrupt for any endpoints that don't already have
   // an IBN pending in IbnFlag
   IBNIE = bmEP6IBN ;
}
void ISR_Ep0pingnak(void) interrupt 0
{
}
void ISR_Ep1pingnak(void) interrupt 0
{
}
void ISR_Ep2pingnak(void) interrupt 0
{
}
void ISR_Ep4pingnak(void) interrupt 0
{
  // if we get a PING-NAK interrupt then arm the endpoint
   SYNCDELAY;                    
   EP4BCL = 0x80;

   EZUSB_IRQ_CLEAR();
   NAKIRQ = bmEP4PING;
}
void ISR_Ep6pingnak(void) interrupt 0
{
}
void ISR_Ep8pingnak(void) interrupt 0
{
}
void ISR_Errorlimit(void) interrupt 0
{
}
void ISR_Ep2piderror(void) interrupt 0
{
}
void ISR_Ep4piderror(void) interrupt 0
{
}
void ISR_Ep6piderror(void) interrupt 0
{
}
void ISR_Ep8piderror(void) interrupt 0
{
}
void ISR_Ep2pflag(void) interrupt 0
{
}
void ISR_Ep4pflag(void) interrupt 0
{
}
void ISR_Ep6pflag(void) interrupt 0
{
}
void ISR_Ep8pflag(void) interrupt 0
{
}
void ISR_Ep2eflag(void) interrupt 0
{
}
void ISR_Ep4eflag(void) interrupt 0
{
}
void ISR_Ep6eflag(void) interrupt 0
{
}
void ISR_Ep8eflag(void) interrupt 0
{
}
void ISR_Ep2fflag(void) interrupt 0
{
}
void ISR_Ep4fflag(void) interrupt 0
{
}
void ISR_Ep6fflag(void) interrupt 0
{
}
void ISR_Ep8fflag(void) interrupt 0
{
}
void ISR_GpifComplete(void) interrupt 0
{
}
void ISR_GpifWaveform(void) interrupt 0
{
}

#pragma NOIV               // Do not generate interrupt vectors
//-----------------------------------------------------------------------------
//   File:      bulksrc.c
//   Contents:   Hooks required to implement USB peripheral function.
//
// $Archive: /USB/Examples/FX2LP/bulksrc/bulksrc.c $
// $Date: 3/23/05 2:56p $
// $Revision: 3 $
//
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
BYTE xdata myBuffer[512];
WORD myBufferCount;
WORD packetSize;

//-----------------------------------------------------------------------------
// Task Dispatcher hooks
//   The following hooks are called by the task dispatcher.
//-----------------------------------------------------------------------------

void TD_Init(void)              // Called once at startup
{
  int i;

  // set the CPU clock to 48MHz
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
  SYNCDELAY;                    // 
  EP4CFG = 0xA0;
  SYNCDELAY;                    // 
  EP6CFG = 0xE2;
  SYNCDELAY;                    // 
  EP8CFG = 0xE0;

  // out endpoints do not come up armed
  
  // since the defaults are double buffered we must write dummy byte counts twice
  SYNCDELAY;                    // 
  EP2BCL = 0x80;                // arm EP2OUT by writing byte count w/skip.
  SYNCDELAY;                    // 
  EP4BCL = 0x80;    
  SYNCDELAY;                    // 
  EP2BCL = 0x80;                // arm EP4OUT by writing byte count w/skip.
  SYNCDELAY;                    // 
  EP4BCL = 0x80;    

  // fill up both IN endpoints

  for (i=0;i<512;i++)
     EP6FIFOBUF[i] = i+2;
  SYNCDELAY;                    // 
  EP6BCH = 0x02;
  SYNCDELAY;                    // 
  EP6BCL = 0x00;

  for (i=0;i<512;i++)
     EP6FIFOBUF[i] = i+2;
  SYNCDELAY;                    // 
  EP6BCH = 0x02;
  SYNCDELAY;                    // 
  EP6BCL = 0x00;

  myBufferCount = 0;

  // enable dual autopointer(s)
  AUTOPTRSETUP |= 0x01;  

}

void TD_Poll(void)              // Called repeatedly while the device is idle
{
   int i;

   // if there is some data in EP2 OUT, re-arm it
   if(!(EP2468STAT & bmEP2EMPTY))
   {
      SYNCDELAY;                // 
      EP2BCL = 0x80;
   }

   // if EP6 IN is available, re-arm it
   if(!(EP2468STAT & bmEP6FULL))
   {
      SYNCDELAY;                // 
      EP6BCH = 0x02;
      SYNCDELAY;                // 
      EP6BCL = 0x00;
   }

   // if there is new data in EP4FIFOBUF, then copy it to a temporary buffer
   if(!(EP2468STAT & bmEP4EMPTY))
   {
      APTR1H = MSB( &EP4FIFOBUF );
      APTR1L = LSB( &EP4FIFOBUF );

      AUTOPTRH2 = MSB( &myBuffer );
      AUTOPTRL2 = LSB( &myBuffer );

      myBufferCount = (EP4BCH << 8) + EP4BCL;

      for( i = 0x0000; i < myBufferCount; i++ )
      {
         EXTAUTODAT2 = EXTAUTODAT1;
      }

      SYNCDELAY;                // 
      EP4BCL = 0x80;            // re(arm) EP4OUT
   }

   // if there is room in EP8IN, then copy the contents of the temporarty buffer to it
   if(!(EP2468STAT & bmEP8FULL) && myBufferCount)
   {
      APTR1H = MSB( &myBuffer );
      APTR1L = LSB( &myBuffer );

      AUTOPTRH2 = MSB( &EP8FIFOBUF );
      AUTOPTRL2 = LSB( &EP8FIFOBUF );

      for( i = 0x0000; i < myBufferCount; i++ )
      {
         // setup to transfer EP4OUT buffer to EP8IN buffer using AUTOPOINTER(s) in SFR space
         EXTAUTODAT2 = EXTAUTODAT1;
      }
      SYNCDELAY;                    // 
      EP8BCH = MSB(myBufferCount);
      SYNCDELAY;                    // 
      EP8BCL = LSB(myBufferCount);  // arm EP8IN
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
   return(TRUE);
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
   if (EZUSB_HIGHSPEED())
   {
      pConfigDscr = pHighSpeedConfigDscr;
      pOtherConfigDscr = pFullSpeedConfigDscr;
      packetSize = 512;

   }
   else
   {
      pConfigDscr = pFullSpeedConfigDscr;
      pOtherConfigDscr = pHighSpeedConfigDscr;
      packetSize = 64;
   }
   
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
      pOtherConfigDscr = pFullSpeedConfigDscr;
      packetSize = 512;

   }
   else
   {
      pConfigDscr = pFullSpeedConfigDscr;
      pOtherConfigDscr = pHighSpeedConfigDscr;
      packetSize = 64;
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
}
void ISR_Ep1out(void) interrupt 0
{
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

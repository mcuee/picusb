/********************************************************************
 FileName:     	winusb.c
 Dependencies:	See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the “Company”) for its PIC® Microcontroller is intended and
 supplied to you, the Company’s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   5/18/2008    Adapted Microchip usb_function_hid.c for use with WinUSB
                     (Jan Axelson)    
  1.3   9/30/2008    Revised code for Control Write transfers.
*******************************************************************/

/** INCLUDES *******************************************************/
#include "GenericTypeDefs.h"
#include "Compiler.h"
#include "usb_config.h"
#include "./USB/usb_device.h"
#include "winusb.h"

/** VARIABLES ******************************************************/
#pragma udata

/** PRIVATE PROTOTYPES *********************************************/

BOOL RequestSupported(void);
void WinusbControlReadTransferHandler(void);
void WinusbControlWriteTransferHandler(void);
void WinusbCtrlTrfTxService(void);

/** DECLARATIONS ***************************************************/
#pragma code

/** CLASS SPECIFIC REQUESTS ****************************************/
/********************************************************************
 * Function:        void USBCheckWinUSBRequest(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine checks the setup data packet to see
 *                  if it knows how to handle it
 *
 * Note:            None
 *******************************************************************/

void USBCheckWinUSBRequest(void)
{
    if(SetupPkt.Recipient != RCPT_INTF) return;
    if(SetupPkt.bIntfID != WINUSB_INTF_ID) return;

    if(SetupPkt.RequestType != VENDOR) return;

	// Vendor-specific requests.

    switch(SetupPkt.bRequest)
    {
        case WINUSB_REQUEST_1:		

			// Data stage is host to device.
			
            WinusbControlWriteTransferHandler();
            break;

        case WINUSB_REQUEST_2:
			
			// Data stage is device to host.

            WinusbControlReadTransferHandler();            
            break;
      
    }//end switch(SetupPkt.bRequest)
   
}//end USBCheckWinUSBRequest

/********************************************************************
 * Function:        void WinUSBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Initialize WinUSB endpoints. Called after the USB
 *                  host has sent out a SET_CONFIGURATION request.
 *                  See USBStdSetCfgHandler() in usbd.c for examples.
 *
 * Note:            None
 *******************************************************************/
#if !defined(USB_DYNAMIC_EP_CONFIG)
void WinUSBInitEP(void)
{   
 	//enable the WinUSB endpoints

    USBEnableEndpoint(WINUSB_INTERRUPT_EP,USB_IN_ENABLED|USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
 	USBEnableEndpoint(WINUSB_BULK_EP,USB_IN_ENABLED|USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);

}//end WinUSBInitEP
#endif

/******************************************************************************
 * Function:        void WinusbControlReadTransferHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:       	Called on receiving a Setup packet for a WinUSB control Read request.
 * 					Calls a function to provide data to send in the Data stage.	
 *
 * Note:            None
 *****************************************************************************/

void WinusbControlReadTransferHandler(void)
{	
	//inPipes[0].info.bits.busy = 1;
	inPipes[0].info.Val = USB_INPIPES_RAM | USB_INPIPES_BUSY | USB_INPIPES_INCLUDE_ZERO;
	
	if ((SetupPkt.bRequest) == 0x02)	
	{							
			inPipes[0].pSrc.bRam = (BYTE*)&winusb_control_in_buffer;
            inPipes[0].wCount.Val = WINUSB_BULK_IN_BYTES;	
	}

	else 
	{
		// Stall unsupported requests.

		pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
	}

}//end WinusbControlReadTransferHandler

/******************************************************************************
 * Function:        void WinusbControlWriteTransferHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:       	Called on receiving a Setup packet for a control Write request.
 *
 * Note:            None
 *****************************************************************************/

void WinusbControlWriteTransferHandler(void)
{
	if (RequestSupported())
	{
        outPipes[0].wCount.Val = SetupPkt.wLength;
        outPipes[0].pDst.bRam = (BYTE*)CONTROL_OUTBUFFER;
        outPipes[0].pFunc = WINUSB_CONTROL_WRITE_PFUNC;
        outPipes[0].info.bits.busy = 1;
	}
	else
	{
		// The endpoint will return STALL.

		outPipes[0].info.bits.busy = 0;
	}	
}

/** EOF winusb.c ******************************************************/

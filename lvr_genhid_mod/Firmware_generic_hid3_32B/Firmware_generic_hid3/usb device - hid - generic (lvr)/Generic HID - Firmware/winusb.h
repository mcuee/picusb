/*******************************************************************************

    USB WinUSB class header file

This file, with its associated C source file, provides the interface required
to talk to the USB host through the WinUSB driver.  

******************************************************************************/
//DOM-IGNORE-BEGIN
/******************************************************************************
 FileName:     	winsub.h
 Dependencies:	See INCLUDES section
 Processor:		PIC18 USB Microcontrollers
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
 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  1.0   8/18/2008    Adapted Microchip file usb_function_hid.h for WinUSB
                     (Jan Axelson)
                     
*******************************************************************/
#ifndef WINUSB_H
#define WINUSB_H
//DOM-IGNORE-END

/** INCLUDES *******************************************************/

/** DEFINITIONS ****************************************************/

/* Vendor-specific Requests */

#define WINUSB_REQUEST_1     0x01
#define WINUSB_REQUEST_2     0x02

#define WINUSB_CONTROL_WRITE_PFUNC &USB_WINUSB_CONTROL_WRITE_HANDLER
#define CONTROL_WRITE_BUFFER &winusb_control_write_buffer[0]


/* WINUSB (vendor-specific) Interface Class Code */

#define WINUSB_INTF                    0xff

#define WinUSBTxHandleBusy USBHandleBusy
#define WinUSBRxHandleBusy USBHandleBusy
#define WinUSBTxPacket USBTxOnePacket
#define WinUSBRxPacket USBRxOnePacket

//===========================
#define WINUSB_CONTROL_OUT_PFUNC &USB_WINUSB_CONTROL_OUT_HANDLER
#define CONTROL_OUTBUFFER &winusb_control_out_buffer[0]
//============================

// Section: STRUCTURES *********************************************/

/** EXTERNS ********************************************************/

/** PUBLIC PROTOTYPES **********************************************/
#if defined(USB_DYNAMIC_EP_CONFIG)
    #define WinUSBInitEP()
#else
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
	 * Overview:        WinUSBInitEP initializes endpoints, buffer
	 *                  descriptors, internal state-machine, and
	 *                  variables. It should be called after the USB
	 *                  host has sent out a SET_CONFIGURATION request.
	 *                  See USBStdSetCfgHandler() in usbd.c for examples.
	 *
	 * Note:            None
	 *******************************************************************/
    void WinUSBInitEP(void);
#endif

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
void USBCheckWinUSBRequest(void);

/** P U B L I C  P R O T O T Y P E S *****************************************/

void USBCheckWinUsbRequest(void);
void WinUsbInitEP(void);

#endif //WINUSB_H

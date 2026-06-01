Generic_HID Firmware for Microchip 18F USB Chips
Using Microchip Libraries for Applications v2012-10-15

Jan Axelson
www.Lvr.com
jan (at) Lvr.com

1/11/13

V0.91

PROJECT DESCRIPTION

This firmware is a modification of Microchip's USB example titled
Device - HID - Custom Demos. The firmware does the following:

Receives HID Output reports using interrupt OUT transfers and sends the 
report data back to the host in HID Input reports using interrupt IN 
transfers.

Receives HID Output reports using control transfers (Set_Report request) and 
sends the report data back to the host in HID Input reports using control 
transfers (Get_Report requests.)

Receives HID Feature reports using control transfers (Set_Report request) 
and sends the report data back to the host in HID Feature reports using 
control transfers (Get_Report requests.)

The project has been tested on the PICDEM™ FS USB DEMONSTRATION BOARD.

MORE INFORMATION

This application, host applications to communicate with the device, and
more about USB and HIDs are available from:

www.Lvr.com/hidpage.htm

HOW TO CREATE THE PROJECT

Download and install the Microchip Libraries for Applications. 

(Search the web for for "Microchip Libraries for Applications"):
 
The framework's installation creates a directory structure under 
"C:\Microchip Solutions v2012-10-15", assuming you use the default directory. 
The version number might be different.

The zip file that contains this readme file contains a project directory 
for the Generic HID application. The directory is:

Device - HID - Generic (LVR)

Copy this directory and its contents as a subdirectory of the Microchip 
Solutions v2012-10-15\USB directory so you end up with:

Microchip Solutions v2012-10-15\USB\Device - HID - Generic (LVR)

USING MPLAB

I tested the firmware using MPLAB v8.88 and MPLAB X v1.60

In MPLAB X, open the project by selecting the project's Firmware\MPLAB.X directory.

In MPLAB 8.88, open the project by selecting the project's workspace (.mcw) file.

THE FILES

This firmware makes changes to these files copied from the Custom Demos project:

main.c
usb_config.h
usb_descriptors.c

and adds the file:

main.h

The firmware requires no changes to the USB stack files in Microchip Solutions v2012-10-15\Microchip\USB. 

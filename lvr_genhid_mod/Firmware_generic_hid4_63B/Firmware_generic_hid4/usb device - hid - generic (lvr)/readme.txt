Generic_HID Firmware for Microchip 18F USB Chips
Using Microchip MCHPFSUSB Framework v2.5a

Jan Axelson
www.Lvr.com
jan (at) Lvr.com

8/17/09

V0.91

PROJECT DESCRIPTION

This firmware is a modification of Microchip's USB Device - HID - Custom
Demos > Generic HID Firmware example for a generic HID USB device (not a
system keyboard or mouse). The firmware does the following:

Receives Output reports using interrupt OUT transfers and sends the 
report data back to the host in Input reports using interrupt IN 
transfers.

Receives Output reports using control transfers (Set_Report request) and 
sends the report data back to the host in Input reports using control 
transfers (Get_Report requests.)

Receives Feature reports using control transfers (Set_Report request) 
and sends the report data back to the host in Feature reports using 
control transfers (Get_Report requests.)

The project has been tested on the PICDEM™ FS USB DEMONSTRATION BOARD.


MORE INFORMATION

This application, host applications to communicate with the device, and
more about USB and HIDs are available from:

www.Lvr.com/hidpage.htm


HOW TO CREATE THE PROJECT

Download and install the Microchip PIC18F and PIC24F MCHPFSUSB Framework 
V2.5. 

The Framework is here (or search for "usb framwork"):
 
http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=2680&dDocName=en537044

The framework's installation creates a directory structure under 
"Microchip Solutions", assuming you use the default directory.

The zip file that contains this readme file contains a project directory 
for the Generic HID application. The directory is:

USB Device - HID - Generic (LVR)

Copy this directory and its contents as a subdirectory of the Microchip 
Solutions directory so you end up with:

Microchip Solutions\USB Device - HID - Generic (LVR)


STACK CHANGES

This firmware requires no changes to the USB stack files in Microchip Solutions\Microchip. 


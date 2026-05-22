Please  check all the below options for firmware examples to work:

1.setenv.bat: under bin folder is modified to

@echo off

REM to create a shortcut to this environment use the command line:
REM COMMAND.COM /e:4096 /k C:\cypress\usb\CY3684_EZ-USB_FX2LP_DVK\1.0\bin\setenv.bat
REM
REM If you install the Keil or Cypress tools in a non-default location 
REM you must update this file

path=c:\cypress\usb\CY3684_EZ-USB_FX2LP_DVK\1.0\bin;c:\keil\c51\bin;%path%
set CYUSB=c:\cypress\usb\CY3684_EZ-USB_FX2LP_DVK\1.0
set C51INC=%CYUSB%\Target\inc;C:\keil\c51\inc

@echo on


Open each firmware example using Keil uvision2 or higher 

2.Under IDE keil-->Project-->"File Extensions,Books and Environment" the paths is changed to

..\..\Target\Inc\;C:\Keil\C51\INC\



3.In the Project "Options for Target Target1" -->Output Tab--> Check if "Run User program1" option is checked.

with the path

..\..\Bin\hex2bix -i -f 0xC2 -o xxx.iic xxx.hex
 
This option allows to generate EEPROM image in .IIC format.

All these changes are under the assumption that CY3684 DVK Installer.exe default installation directory
is
c:\cypress\usb\CY3684_EZ-USB_FX2LP_DVK\1.0

During Installation if the install directory is changed by the end user 
then steps 1 are to be repeated with modified path


4.During compilation of firmware examples if EZUSB.Lib and USBJmptbl.obj are not added
  and results in error during project build.Select "Source Group1" folder in the Project Window
  Right click in project window and select "Add files to Group 'Source Group1' and browse 
  to following folder for respctive kits

   FX1:C:\Cypress\USB\CY3674_EZ-USB_FX1_DVK\<ver>\Target\Lib\LP
   FX2LP:C:\Cypress\USB\CY3684_EZ-USB_FX2LP_DVK\<ver>\Target\Lib\LP

  Note: if the path of installation directory is changed then relevant path must be chosen
	FX1:<Installed_directory>\CY3674_EZ-USB_FX1_DVK\<ver>\Target\Lib\LP
	FX2LP:<Installed_directory>\CY3684_EZ-USB_FX2LP_DVK\<ver>\Target\Lib\LP

  In the browse window select Files to All types .Then only EZUSB.Lib and USBJmptbl.obj files are visible
  Press Ctrl key on your keyboard and select both these files at the same instant.
  Observe files getting added to project window.Build the project again and observe the build errors
  no longer exist.

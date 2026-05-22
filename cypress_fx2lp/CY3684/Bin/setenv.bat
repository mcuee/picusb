@echo off

REM to create a shortcut to this environment use the command line:
REM COMMAND.COM /e:4096 /k C:\cypress\usb\CY3684_EZ-USB_FX2LP_DVK\1.0\bin\setenv.bat
REM
REM If you install the Keil or Cypress tools in a non-default location 
REM you must update this file

path=c:\cypress\usb\CY3684_EZ-USB_FX2LP_DVK\1.0\Bin;c:\keil\c51\bin;%path%
set CYUSB=c:\cypress\usb\CY3684_EZ-USB_FX2LP_DVK\1.0\
set C51INC=%CYUSB%\Target\inc;C:\keil\c51\inc

@echo on
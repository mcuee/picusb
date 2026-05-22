@echo off
REM #--------------------------------------------------------------------------
REM #	File:		BUILD.BAT
REM #	Contents:	Batch file to build firmware
REM #
REM # $Archive: /USB/Examples/Fx2lp/dev_io/build.bat $
REM # $Date: 9/01/03 8:54p $
REM # $Revision: 2 $
REM #
REM #
REM #-----------------------------------------------------------------------------
REM # Copyright 2003, Cypress Semiconductor Corporation
REM #-----------------------------------------------------------------------------

REM # command line switches
REM # ---------------------
REM # -clean delete temporary files

REM ## Compile C source file ##
c51 dev_io.c debug objectextend small moddp2

REM ## Link binary object with debug info ##
echo dev_io.obj, %CYUSB%\Target\Lib\LP\EZUSB.lib  > tmp.rsp
echo TO dev_io RAMSIZE(256)  PL(68) PW(78) CODE(0000h) XDATA(1000h)  >> tmp.rsp
bl51 @tmp.rsp

REM ## Generate intel hex image without debug info ##
oh51 dev_io HEXFILE(dev_io.hex)

REM ### usage: build -clean to remove intermediate files after build
if "%1" == "-clean" del *.lst
if "%1" == "-clean" del *.obj
if "%1" == "-clean" del *.m51

; dscr.a51
; Contains the Device Descriptor, Configuration(Interface, HID and Endpoint) Descriptor,
; and String descriptors.
;


DSCR_DEVICE   	 equ   1   			;; Descriptor type: Device
DSCR_CONFIG  	 equ   2   			;; Descriptor type: Configuration
DSCR_STRING   	 equ   3  		 	;; Descriptor type: String
DSCR_INTRFC   	 equ   4   			;; Descriptor type: Interface
DSCR_ENDPNT   	 equ   5   			;; Descriptor type: Endpoint
DSCR_DEVQUAL  	 equ   6   			;; Descriptor type: Device Qualifier
DSCR_OTHERSPEED  equ   7 


ET_CONTROL   equ   0   				;; Endpoint type: Control
ET_ISO       equ   1   				;; Endpoint type: Isochronous
ET_BULK      equ   2   				;; Endpoint type: Bulk
ET_INT       equ   3   				;; Endpoint type: Interrupt

public	DeviceDscr,ConfigDscr,StringDscr,HIDDscr,ReportDscr,ReportDscrEnd,StringDscr0, StringDscr1, StringDscr2
public  HighSpeedConfigDscr,FullSpeedConfigDscr,DeviceQualDscr, UserDscr
 
;EZ USB FX2 control panel Vendor ID 0h, product ID 2131h
; Lakeview Research VID is 0925h.   
VID	equ	0925h
PID	equ	1234h
DID 	equ	0000h

;cseg at 0x90
DSCR   SEGMENT   CODE PAGE

;;-----------------------------------------------------------------------------
;; Global Variables
;;-----------------------------------------------------------------------------
      rseg DSCR      ;; locate the descriptor table in on-part memory.

DeviceDscr:	
	db	DeviceDscrEnd - DeviceDscr		; Descriptor length
	db	DSCR_DEVICE			; Descriptor type = DEVICE
	db	00h,02h				; spec version (BCD) is 2.00               
	db	0,0,0				; HID class is defined in the interface descriptor
	db	64				; maxPacketSize
	db	LOW(VID),HIGH(VID)
	db	LOW(PID),HIGH(PID)
	db	LOW(DID),HIGH(DID)
	db  	 1        			; Manufacturer string index
      	db  	 2         			; Product string index
      	db  	 0         			; Serial number string index
      	db  	 1         			; Number of configurations
DeviceDscrEnd:

DeviceQualDscr:
    db  DeviceQualDscrEnd - DeviceQualDscr      ;Descriptor Length
    db  DSCR_DEVQUAL        			;Descriptor Type
    db  00h,02h            			;spec version (BCD) is 2.00
    db  0,0,0               			;Device class, sub-class, and sub-sub-class
    db  04h                 			;Max Packet Size
    db  1                  		 	;Number of configurations
    db  0                   			;Reserved
DeviceQualDscrEnd:

HighSpeedConfigDscr:
ConfigDscr:
	db	ConfigDscrEnd - ConfigDscr	; Descriptor length
        db	DSCR_CONFIG			; Descriptor type = CONFIG
	db	LOW(HS_End-ConfigDscr)		; total length (conf+interface+HID+EP's)
	db	HIGH(HS_End-ConfigDscr)
	db	01h				; number of interfaces
	db	01h				; value to select this interface
	db	03h				; string index to describe this config
	db	10000000b			; b7=1; b6=self-powered; b5=Remote WU
	db	40d				; bus power = 80 ma
ConfigDscrEnd:

IntrfcDscr:		; Interface Descriptor
	db	IntrfcDscrEnd -  IntrfcDscr	; Descriptor length
	db	DSCR_INTRFC			; Descriptor type = INTERFACE
	db	0,0				; Interface 0, Alternate setting 0
	db	02h				; number of endpoints
	db	03h,0,0				; class(03)HID, no subclass or protocol
	db	0h				; string index for this interface
IntrfcDscrEnd:

HIDDscr:
	db	HIDDscrEnd - HIDDscr		; Descriptor length
	db	21h				; Descriptor type - HID
	db	10h,01h				; HID Spec version 1.10
	db	0				; country code(none)
	db	01h				; number of HID class descriptors
	db	22h				; class descriptor type: REPORT
	db	LOW(ReportDscrEnd - ReportDscr)
        db	HIGH(ReportDscrEnd - ReportDscr)
HIDDscrEnd:

EpInDscr:	; I-0, AS-0 first endpoint descriptor (EP1IN)
	db	EpInDscrEnd - EpInDscr		; Descriptor length
	db	DSCR_ENDPNT			; Descriptor type = ENDPOINT
	db	81h				; IN-1
	db	03h				; Type: INTERRUPT
	db	64d,0				; MaxPacketSize = 64
	db	05h				; polling interval is 2^(5-1) = 16 mSec
EpInDscrEnd:

EpOutDscr:
	db	EpOutDscrEnd - EpOutDscr	; Descriptor length
	db	DSCR_ENDPNT			; Descriptor type = ENDPOINT
	db	01h				; OUT-1
	db	03h				; Type ; INTERRUPT
	db	64d,0				; MaxPacketSize = 64
	db	05h				; polling interval is 2^(5-1) = 16 mSec
EpOutDscrEnd:	
HS_End:

    db  00h         ; Word alignment

FullSpeedConfigDscr:
	db	FullSpeedConfigDscrEnd - FullSpeedConfigDscr		; Descriptor length
	db	DSCR_OTHERSPEED			; Descriptor type = OTHER SPEED CONFIG
	db	LOW(FS_End-FullSpeedConfigDscr)	; Total length (conf+interface+HID+EP's)
	db	HIGH(FS_End-FullSpeedConfigDscr)
	db	01h				; Number of interfaces
	db	01h				; Value to select this interface
	db	03h				; String index to describe this config
	db	10100000b			; b7=1; b6=self-powered; b5=Remote WU
	db	0d				; bus power = 80 ma
FullSpeedConfigDscrEnd:
 
FullSpeedIntrfcDscr:				; Interface Descriptor
	db	FullSpeedIntrfcDscrEnd -  FullSpeedIntrfcDscr	; Descriptor length
	db	DSCR_INTRFC			; Descriptor type: INTERFACE
	db	0,0				; Interface 0, Alternate setting 0
	db	02h				; Number of endpoints
	db	03h,0,0				; Class(03)HID, no subclass or protocol
	db	0h				; string index for this interface
FullSpeedIntrfcDscrEnd:


FullSpeedHIDDscr:
	db	FullSpeedHIDDscrEnd -FullSpeedHIDDscr	; Descriptor length
	db	21h				; Descriptor type - HID
	db	10h,01h				; HID Spec version 1.10
	db	0				; country code(none)
	db	01h				; number of HID class descriptors
	db	22h				; class descriptor type: REPORT
	db	LOW(ReportDscrEnd - ReportDscr)
	db	HIGH(ReportDscrEnd - ReportDscr)
FullSpeedHIDDscrEnd:

FSEpInDscr:	
	db	FSEpInDscrEnd - FSEpInDscr	; Descriptor length
	db	DSCR_ENDPNT			; Descriptor type : ENDPOINT
	db	81h				; IN-1
	db	ET_INT				; Type: INTERRUPT
	db	40h,0				; maxPacketSize = 64
	db	01h				; polling interval is 50 msec
FSEpInDscrEnd:

FSEpOutDscr:	
	db	FSEpOutDscrEnd - FSEpOutDscr	; Descriptor length
	db	DSCR_ENDPNT			; Descriptor type = ENDPOINT
	db	01h				; OUT-1
	db	ET_INT				; type - INTERRUPT
	db	40h,0				; maxPacketSize = 12
	db	01h				; polling interval is 50 msec
FSEpOutDscrEnd:
FS_End:

    	db  00h         ;Word alignment

;; usbhidio code start
ReportDscr:

	db 06h, 0A0h, 0FFh ;    Usage Page (FFA0h, vendor defined)
	db 09h, 01h     ;       Usage (vendor defined)
	db 0A1h, 01h    ;       Collection (Application)
	db 09h, 02h     ;       Usage (vendor defined)
	db 0A1h, 00h    ;       Collection (Physical)
	db 06h, 0A1h, 0FFh ;    Usage Page (vendor defined)

;; The Input report
	db 09h, 03h     ;       Usage (vendor defined)
	db 09h, 04h     ;       Usage (vendor defined)
	db 15h, 80h	;	Logical minimum (80h or -128)
	db 25h, 7Fh	;	Logical maximum (7Fh or 127)
	db 35h, 00h	;	Physical minimum (0)
	db 45h, 0FFh	;	Physical maximum (255)
	db 75h, 08h	;	Report size (8 bits)
	db 95h, 02h	;	Report count (2 fields)
	db 81h, 02h	;	Input (data, variable, absolute)

;; The Output report
	db 09h, 05h     ;       Usage (vendor defined)
	db 09h, 06h     ;       Usage (vendor defined)
	db 15h, 80h	;	Logical minimum (80h or -128)
	db 25h, 7Fh	;	Logical maximum (7Fh or 127)
	db 35h, 00h	;	Physical minimum (0)
	db 45h, 0FFh	;	Physical maximum (255)
	db 75h, 08h	;	Report size (8 bits)
	db 95h, 02h	;	Report count (2 fields)
	db 91h, 02h	;	Output (data, variable, absolute)

	db 0C0h         ;       End Collection (Physical)
	db 0C0h         ;       End Collection (Application)

ReportDscrEnd:

ReportEnd_word_allignment:
;; Alignment unneeded for usbhidio report because it has an even number of bytes.
;;   	db  00h         ;Force word alignment

;; usbhdio code end

/* Descriptor for Cypress keyboard example (unused by usbhidio)
ReportDscr:
	db 05h, 01h     ; Usage Page (Generic Desktop)
	db 09h, 06h     ; Usage (Keyboard)
	db 0A1h, 01h     ; Collection (Application)
	db 05h, 07h     ;       Usage Page (Key codes)
	db 19h, 0E0h	;	Usage minimum (234)
	db 29h, 0E7h	;	Usage maximum (231)
	db 15h, 00h	;	Logical minimum (0)
	db 25h, 01h	;	Logical maximum (1)
	db 75h, 01h	;	Report size (1)
	db 95h, 08h	;	Report count (8)
	db 81h, 02h	;	Input (data, variable, absolute)
	db 95h, 01h	;	Report count (1)
	db 75h, 08h	;	Report size (8)
	db 81h, 01h	;	Input (constant)
	db 95h, 05h	;	Report count (5)
	db 75h, 01h	;	Report size (1)
	db 05h, 08h	;	Usage Page (LED)
	db 19h, 01h	;	Usage minimum (1)
	db 29h, 05h	;	Usage maximum (5)
	db 91h, 02h	;	Output (data, variable, absolute)
	db 95h, 01h	;	Report count (1)
	db 75h, 03h	;	Report size (3)
	db 91h, 01h	;	Output (constant)
	db 95h, 03h	;	Report count (3)
	db 75h, 08h	;	Report size (8)
	db 15h, 00h	;	Logical minimum (0)
	db 25h, 65h	;	Logical maximum (101)
	db 05h, 07h	;	Usage page (key codes)
	db 19h, 00h	;	Usage minimum (0)
	db 29h, 65h	;	Usage maximum (101)
	db 81h, 00h	;	Input (data, array)
	db 0C0h          ; End Collection
ReportDscrEnd:

ReportEnd_word_allignment:
   	db  00h         ;Force word alignment

*/

StringDscr:
StringDscr0:
		db	StringDscr0End-StringDscr0		;; String descriptor length
		db	DSCR_STRING
		db	09H,04H
StringDscr0End:

StringDscr1:	
		db	StringDscr1End-StringDscr1		;; String descriptor length
		db	DSCR_STRING
		db	'C',00
		db	'Y',00
		db	'P',00
		db	'R',00
		db	'E',00
		db	'S',00
		db	'S',00

StringDscr1End:
StringDscr2:	
		db	StringDscr2End-StringDscr2		;; Descriptor length
		db	DSCR_STRING
		db	'E',00
		db	'Z',00
		db	'-',00
		db	'U',00
		db	'S',00
		db	'B',00
		db	' ',00
		db	'F',00
		db	'X',00
		db	'2',00
		db	' ',00
		db	'H',00
		db	'I',00
		db	'D',00
		db	' ',00
		db	'U',00
		db	'S',00
		db	'B',00
		db	'H',00
		db	'I',00
		db	'D',00
		db	'I',00
		db	'O',00
	
StringDscr2End:
	
UserDscr:		
		dw	0000H
		end



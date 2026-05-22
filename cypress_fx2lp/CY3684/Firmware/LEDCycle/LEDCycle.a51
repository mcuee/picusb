;--------------------------------------------------------------
; LEDCycle.A51
;
; Check for signs of life on FX2 IO ports
;--------------------------------------------------------------
$NOMOD51		; disable predefined 8051 registers
$nolist
$INCLUDE (fx2regs.inc)	; *** for the Dallas (Synopsys) part
$list
;
NAME		portio
;
		ISEG AT 60H		; stack
stack:		ds	20
;
		DSEG at 20H		; bit-addressable
litebits:	ds	1
;
		CSEG	AT	0	; absolute Segment at Address 0
		LJMP	start		; Jump over the interrupt vectors
; -------------------------------------------------
		org	1400h		
; -------------------------------------------------
start:		mov	SP,#STACK-1	; set stack
;
lth:		call	cycle_lites
		sjmp	lth
;
cycle_lites:
      mov	dptr,#8800h  ; LED2 ON
		movx	a,@dptr
		call	delay
		mov	dptr,#9800h  ; LED3 ON
		movx	a,@dptr
		call	delay
		mov	dptr,#0A800h  ; LED4 ON
		movx	a,@dptr
		call	delay
		mov	dptr,#0B800h  ; LED5 ON
		movx	a,@dptr
		call	delay
		mov	dptr,#8000h   ; LED2 OFF
		movx	a,@dptr
		call	delay
		mov	dptr,#9000h   ; LED3 OFF
		movx	a,@dptr
		call	delay
		mov	dptr,#0A000h   ; LED4 OFF
		movx	a,@dptr
		call	delay
		mov	dptr,#0B000h   ; LED5 OFF
		movx	a,@dptr
		call	delay
		ret
;
delay:		mov	r2,#4
dl1:		djnz	r0,dl1
		djnz	r1,dl1
		djnz	r2,dl1
		ret
;
		END


;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (Linux)
;--------------------------------------------------------
; PIC port for the 14-bit core
;--------------------------------------------------------
;	.file	"blink.c"
	list	p=12f675
	radix dec
	include "p12f675.inc"
;--------------------------------------------------------
; external declarations
;--------------------------------------------------------
	extern	__modsint
	extern	_TRISIO
	extern	_GPIO
	extern	_GPIObits
	extern	__sdcc_gsinit_startup
;--------------------------------------------------------
; global declarations
;--------------------------------------------------------
	global	_main
	global	_delay
	global	_generarAleatorio

	global PSAVE
	global SSAVE
	global WSAVE
	global STK12
	global STK11
	global STK10
	global STK09
	global STK08
	global STK07
	global STK06
	global STK05
	global STK04
	global STK03
	global STK02
	global STK01
	global STK00

sharebank udata_ovr 0x0020
PSAVE	res 1
SSAVE	res 1
WSAVE	res 1
STK12	res 1
STK11	res 1
STK10	res 1
STK09	res 1
STK08	res 1
STK07	res 1
STK06	res 1
STK05	res 1
STK04	res 1
STK03	res 1
STK02	res 1
STK01	res 1
STK00	res 1

;--------------------------------------------------------
; global definitions
;--------------------------------------------------------
;--------------------------------------------------------
; absolute symbol definitions
;--------------------------------------------------------
;--------------------------------------------------------
; compiler-defined variables
;--------------------------------------------------------
UDL_blink_0	udata
r0x1012	res	1
r0x100B	res	1
r0x100A	res	1
r0x100C	res	1
r0x100D	res	1
r0x100E	res	1
r0x100F	res	1
r0x1010	res	1
r0x1011	res	1
r0x1001	res	1
r0x1002	res	1
r0x1003	res	1
r0x1004	res	1
r0x1005	res	1
r0x1006	res	1
r0x1007	res	1
r0x1008	res	1
;--------------------------------------------------------
; initialized data
;--------------------------------------------------------

IDD_blink_0	idata
_generarAleatorio_semilla_65536_12
	db	0x55	; 85	'U'

;--------------------------------------------------------
; initialized absolute data
;--------------------------------------------------------
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
;	udata_ovr
;--------------------------------------------------------
; reset vector 
;--------------------------------------------------------
STARTUP	code 0x0000
	nop
	pagesel __sdcc_gsinit_startup
	goto	__sdcc_gsinit_startup
;--------------------------------------------------------
; code
;--------------------------------------------------------
code_blink	code
;***
;  pBlock Stats: dbName = M
;***
;has an exit
;functions called:
;   _generarAleatorio
;   _delay
;   _generarAleatorio
;   _delay
;2 compiler assigned registers:
;   r0x1012
;   STK00
;; Starting pCode block
S_blink__main	code
_main:
; 2 exit points
;	.line	7; "blink.c"	TRISIO = 0b00100000; // Poner GP0 como entrada (para el botón) y el resto como salidas
	MOVLW	0x20
	BANKSEL	_TRISIO
	MOVWF	_TRISIO
;	.line	8; "blink.c"	GPIO = 0x00; // Poner todos los pines en bajo
	BANKSEL	_GPIO
	CLRF	_GPIO
_00115_DS_:
;	.line	14; "blink.c"	if (GP5 == 0) { // Si el botón es presionado
	BANKSEL	_GPIObits
	BTFSC	_GPIObits,5
	GOTO	_00115_DS_
;	.line	15; "blink.c"	valor = generarAleatorio();
	PAGESEL	_generarAleatorio
	CALL	_generarAleatorio
	PAGESEL	$
	MOVWF	r0x1012
;	.line	18; "blink.c"	GPIO = 0x00;
	BANKSEL	_GPIO
	CLRF	_GPIO
;;unsigned compare: left < lit(0x1=1), size=1
;	.line	20; "blink.c"	switch (valor) {
	MOVLW	0x01
	SUBWF	r0x1012,W
	BTFSS	STATUS,0
	GOTO	_00111_DS_
;;genSkipc:3307: created from rifx:0x7ffe481e36b0
;;swapping arguments (AOP_TYPEs 1/2)
;;unsigned compare: left >= lit(0x7=7), size=1
	MOVLW	0x07
	SUBWF	r0x1012,W
	BTFSC	STATUS,0
	GOTO	_00111_DS_
;;genSkipc:3307: created from rifx:0x7ffe481e36b0
	DECF	r0x1012,F
	MOVLW	HIGH(_00137_DS_)
	MOVWF	PCLATH
	MOVLW	_00137_DS_
	ADDWF	r0x1012,W
	BTFSC	STATUS,0
	INCF	PCLATH,F
	MOVWF	PCL
_00137_DS_:
	GOTO	_00105_DS_
	GOTO	_00106_DS_
	GOTO	_00107_DS_
	GOTO	_00108_DS_
	GOTO	_00109_DS_
	GOTO	_00110_DS_
_00105_DS_:
;	.line	22; "blink.c"	GP1 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,1
;	.line	23; "blink.c"	break;
	GOTO	_00111_DS_
_00106_DS_:
;	.line	26; "blink.c"	GP2 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,2
;	.line	27; "blink.c"	break;
	GOTO	_00111_DS_
_00107_DS_:
;	.line	30; "blink.c"	GP1 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,1
;	.line	31; "blink.c"	GP2 = 1;
	BSF	_GPIObits,2
;	.line	32; "blink.c"	break;
	GOTO	_00111_DS_
_00108_DS_:
;	.line	35; "blink.c"	GP2 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,2
;	.line	36; "blink.c"	GP4 = 1;
	BSF	_GPIObits,4
;	.line	37; "blink.c"	break;
	GOTO	_00111_DS_
_00109_DS_:
;	.line	40; "blink.c"	GP1 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,1
;	.line	41; "blink.c"	GP2 = 1;
	BSF	_GPIObits,2
;	.line	42; "blink.c"	GP4 = 1;
	BSF	_GPIObits,4
;	.line	43; "blink.c"	break;
	GOTO	_00111_DS_
_00110_DS_:
;	.line	46; "blink.c"	GP1 = 1;
	BANKSEL	_GPIObits
	BSF	_GPIObits,1
;	.line	47; "blink.c"	GP2 = 1;
	BSF	_GPIObits,2
;	.line	48; "blink.c"	GP3 = 1;
	BSF	_GPIObits,3
;	.line	49; "blink.c"	GP4 = 1;
	BSF	_GPIObits,4
_00111_DS_:
;	.line	53; "blink.c"	delay(500); // Mostrar el valor por medio segundo
	MOVLW	0xf4
	MOVWF	STK00
	MOVLW	0x01
	PAGESEL	_delay
	CALL	_delay
	PAGESEL	$
;	.line	54; "blink.c"	GPIO = 0x00; // Apagar todos los LEDs
	BANKSEL	_GPIO
	CLRF	_GPIO
	GOTO	_00115_DS_
;	.line	57; "blink.c"	}
	RETURN	
; exit point of _main

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;functions called:
;   __modsint
;   __modsint
;12 compiler assigned registers:
;   r0x1001
;   r0x1002
;   r0x1003
;   r0x1004
;   r0x1005
;   r0x1006
;   r0x1007
;   r0x1008
;   r0x1009
;   STK02
;   STK01
;   STK00
;; Starting pCode block
S_blink__generarAleatorio	code
_generarAleatorio:
; 2 exit points
;	.line	69; "blink.c"	unsigned char aleatorio = 0;
	CLRF	r0x1001
;	.line	71; "blink.c"	for (int i = 0; i < 8; i++) {
	CLRF	r0x1002
	CLRF	r0x1003
;;signed compare: left < lit(0x8=8), size=2, mask=ffff
_00178_DS_:
	MOVF	r0x1003,W
	ADDLW	0x80
	ADDLW	0x80
	BTFSS	STATUS,2
	GOTO	_00195_DS_
	MOVLW	0x08
	SUBWF	r0x1002,W
_00195_DS_:
	BTFSC	STATUS,0
	GOTO	_00176_DS_
;;genSkipc:3307: created from rifx:0x7ffe481e36b0
;	.line	72; "blink.c"	if ((semilla & 0x01) ^ ((semilla & 0x02) >> 1))
	MOVF	_generarAleatorio_semilla_65536_12,W
	MOVWF	r0x1004
	CLRF	r0x1005
	MOVLW	0x01
	ANDWF	r0x1004,W
	MOVWF	r0x1006
	CLRF	r0x1007
	MOVLW	0x02
	ANDWF	r0x1004,F
	CLRF	r0x1005
;;shiftRight_Left2ResultLit:5474: shCount=1, size=2, sign=1, same=0, offr=0
	BCF	STATUS,0
	BTFSC	r0x1005,7
	BSF	STATUS,0
	RRF	r0x1005,W
	MOVWF	r0x1008
	RRF	r0x1004,W
;;1	MOVWF	r0x1009
	XORWF	r0x1006,F
	MOVF	r0x1008,W
	XORWF	r0x1007,F
	MOVF	r0x1007,W
	IORWF	r0x1006,W
	BTFSC	STATUS,2
	GOTO	_00175_DS_
;;102	MOVF	r0x1002,W
;	.line	73; "blink.c"	aleatorio |= (1 << i);
	MOVLW	0x01
	MOVWF	r0x1004
;;101	MOVF	r0x1006,W
	MOVF	r0x1002,W
	MOVWF	r0x1006
	BTFSC	r0x1006,7
	GOTO	_00199_DS_
	SUBLW	0x00
	BTFSC	STATUS,2
	GOTO	_00197_DS_
_00196_DS_:
	RLF	r0x1004,F
	ADDLW	0x01
	BTFSS	STATUS,0
	GOTO	_00196_DS_
	GOTO	_00197_DS_
_00199_DS_:
	BCF	STATUS,0
	BTFSC	r0x1004,7
	BSF	STATUS,0
	RRF	r0x1004,F
	ADDLW	0x01
	BTFSS	STATUS,0
	GOTO	_00199_DS_
_00197_DS_:
	MOVF	r0x1001,W
	MOVWF	r0x1006
	IORWF	r0x1004,W
	MOVWF	r0x1001
;;shiftRight_Left2ResultLit:5474: shCount=1, size=1, sign=0, same=1, offr=0
_00175_DS_:
;	.line	75; "blink.c"	semilla >>= 1;
	BCF	STATUS,0
	RRF	_generarAleatorio_semilla_65536_12,F
;	.line	71; "blink.c"	for (int i = 0; i < 8; i++) {
	INCF	r0x1002,F
	BTFSC	STATUS,2
	INCF	r0x1003,F
	GOTO	_00178_DS_
_00176_DS_:
;	.line	78; "blink.c"	semilla = aleatorio;
	MOVF	r0x1001,W
	MOVWF	_generarAleatorio_semilla_65536_12
;;100	MOVF	r0x1001,W
;	.line	81; "blink.c"	return (aleatorio % 6) + 1;
	CLRF	r0x1003
	MOVLW	0x06
	MOVWF	STK02
	MOVLW	0x00
	MOVWF	STK01
;;99	MOVF	r0x1002,W
	MOVF	r0x1001,W
	MOVWF	r0x1002
	MOVWF	STK00
	MOVLW	0x00
	PAGESEL	__modsint
	CALL	__modsint
	PAGESEL	$
	MOVWF	r0x1002
	MOVF	STK00,W
	MOVWF	r0x1001
	MOVWF	r0x1003
	INCF	r0x1003,W
	MOVWF	r0x1001
;	.line	82; "blink.c"	}
	RETURN	
; exit point of _generarAleatorio

;***
;  pBlock Stats: dbName = C
;***
;has an exit
;9 compiler assigned registers:
;   r0x100A
;   STK00
;   r0x100B
;   r0x100C
;   r0x100D
;   r0x100E
;   r0x100F
;   r0x1010
;   r0x1011
;; Starting pCode block
S_blink__delay	code
_delay:
; 2 exit points
;	.line	59; "blink.c"	void delay(unsigned int tiempo) {
	MOVWF	r0x100A
	MOVF	STK00,W
	MOVWF	r0x100B
;	.line	63; "blink.c"	for (i = 0; i < tiempo; i++)
	CLRF	r0x100C
	CLRF	r0x100D
_00148_DS_:
	MOVF	r0x100A,W
	SUBWF	r0x100D,W
	BTFSS	STATUS,2
	GOTO	_00169_DS_
	MOVF	r0x100B,W
	SUBWF	r0x100C,W
_00169_DS_:
	BTFSC	STATUS,0
	GOTO	_00150_DS_
;;genSkipc:3307: created from rifx:0x7ffe481e36b0
;	.line	64; "blink.c"	for (j = 0; j < 1275; j++);
	MOVLW	0xfb
	MOVWF	r0x100E
	MOVLW	0x04
	MOVWF	r0x100F
_00146_DS_:
	MOVLW	0xff
	ADDWF	r0x100E,W
	MOVWF	r0x1010
	MOVLW	0xff
	MOVWF	r0x1011
	MOVF	r0x100F,W
	BTFSC	STATUS,0
	INCFSZ	r0x100F,W
	ADDWF	r0x1011,F
	MOVF	r0x1010,W
	MOVWF	r0x100E
	MOVF	r0x1011,W
	MOVWF	r0x100F
	MOVF	r0x1011,W
	IORWF	r0x1010,W
	BTFSS	STATUS,2
	GOTO	_00146_DS_
;	.line	63; "blink.c"	for (i = 0; i < tiempo; i++)
	INCF	r0x100C,F
	BTFSC	STATUS,2
	INCF	r0x100D,F
	GOTO	_00148_DS_
_00150_DS_:
;	.line	65; "blink.c"	}
	RETURN	
; exit point of _delay


;	code size estimation:
;	  179+   17 =   196 instructions (  426 byte)

	end

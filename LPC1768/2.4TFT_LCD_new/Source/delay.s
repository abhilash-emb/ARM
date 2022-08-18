	AREA DELAY, CODE, READONLY
delay4 PROC
	EXPORT delay4
	subs R0,R0,#1
	bne delay4
	BX LR
	ENDP
	END
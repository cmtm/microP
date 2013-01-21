	AREA text, CODE, READONLY
	EXPORT addingFunc
addingFunc
	mov   R1, #13
	mov   R2, #45
	add   R3, R1, R2
	BX LR
	END
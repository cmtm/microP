	AREA text, CODE, READONLY
	EXPORT addingFunc

addingFunc
	VMOV.F32  S0, #2
	vmov.f32  S1, #1.5
	vadd.f32  S3, S0, S1
	BX LR
	END

; addingFunc
	; mov   R1, #13
	; mov   R2, #45
	; add   R3, R1, R2
	; BX LR
	; END
	AREA text, CODE, READONLY
	EXPORT moving_average_init
	EXPORT moving_average
;	EXPORT moving_average_test

; d is the filter depth
d EQU 5
	
; stSz is the size of the state struct in words
stSz EQU d + 3


; State Struct
	; one word for "accumulated"
	; one word for "yPrevious"
	; one word for "tail"
	; d words for circular buffer

; relative addresses
tail      EQU 0     * 4
circBuf   EQU (1)   * 4
accum     EQU (d+1) * 4
yPrevious EQU (d+2) * 4



; function just sets all the data to zero
; pointer to state struct is in r0
moving_average_init
	; prep registers
	mov.W   r1, #0x00000000
	mov.W   r2, #stSz - 1
clearWord
	; fill RAM with 0
	str     r1, [r0, r2, LSL #2]
	; decrement counter
	subs    r2, r2, #1
	; repeat of there is more to do
	BPL     clearWord
	; set the tail pointer.
	;add     r0, r0, #4
	;str     r0, r0
	BX LR
	
; CONSIDER GETTING RID OF ACCUM, it's useless now!
; function where the moving average is actually computed
; pointer to state struct is in r0
; new sample value is in s0
moving_average
	; fetch accum
	LDR   r1, [r0, #accum]
	; increment if needed
	cmp   r1, #d
	BEQ   labelA
	add   r1, #1
	; store accum
	STR       r1, [r0, #accum]
labelA
	; fetch yPrevious
	VLDR.32   s1, [r0, #yPrevious]
	; fetch tail pointer, it's the first word of struct
	LDR       r2, [r0]
	; fetch tail value
	ADD       r1, r0, r2
	VLDR.32   s2, [r1]
	; store new sample at tail pointer
	VSTR.32   s0, [r1]
	; compute y New
	VADD.F32  s0, s0, s1
	VSUB.F32  s0, s0, s2
	; move tail pointer
	SUBS      r2, r2, #4
	; gotta loop it around if needed
	BPL labelB
	ADD       r2, r2, #(d*4)
labelB
	; store computed y as yPrevious
	VSTR.32   s0, [r0, #yPrevious]
	; store new tail pointer
	STR       r2, [r0]
	; Finally, divide y by accum to make average
	; Here, I just divided by d to save some steps.
	; it'll only effect a few values at the beginning.
	VMOV.F32  s1, #5.0
	VDIV.F32  s0, s0, s1
	BX LR
	END
	
	
	
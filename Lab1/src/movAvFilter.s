	AREA text, CODE, READONLY
	EXPORT moving_average_init
	EXPORT moving_average

; d is the filter depth
; it exists independantly in two other places.
; if changing this one, don't forget to change the other two
; one is as a #define in main.c
; the other is as a float literal at the bottom of this file
d EQU 10
	
; stSz is the size of the state struct in words
stSz EQU d + 2


; State Struct
	; one word for "tail"
	; d words for circular buffer
	; one word for "accumulated"
	; one word for "yPrevious"
	
	

; relative addresses
circBuf   EQU (0)   * 4
tail      EQU (d)   * 4
yPrevious EQU (d+1) * 4



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
	
; function where the moving average is actually computed
; pointer to state struct is in r0
; new sample value is in s0
moving_average
	; fetch yPrevious
	VLDR.32   s1, [r0, #yPrevious]
	; fetch tail pointer
	LDR       r2, [r0, #tail]
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
	STR       r2, [r0, #tail]
	; Here, I just divided by d to save some steps.
	; it'll only sqew a few values at the beginning.
	; I have a previous revision of using another
	; variable called accum which counted to d but
	; it was a lot of extra computation for no return
	VMOV.F32  s1, #10.0
	VDIV.F32  s0, s0, s1
	BX LR
	END
	
	
	
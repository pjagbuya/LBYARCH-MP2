; assmebly with x86-65

; 32-bit ints parameters automatically placed 1st, 2nd param, etc.: ecx, edx, r8d
; Floating points- single or double parameters:  xmm0, xmm1, xmm2, etc.


section .data
msg db "Hello World ASM HERE", 13, 10, 0
var1 dd 1.0
section .text
bits 64
default rel   ; to handle address relocation



global imgCvtGrayFloatToInt
extern printf

imgCvtGrayFloatToInt:
	
	movss xmm2, [var1]
	addss xmm0, xmm2
	movss dword[rdx], xmm0

	ret





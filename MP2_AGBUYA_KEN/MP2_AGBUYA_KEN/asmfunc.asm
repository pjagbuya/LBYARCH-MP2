; G11 Member: Paul Josef P. Agbuya  S12
; G11 Member: Ken Ivan T. Cheng


; 32-bit ints parameters automatically placed 1st, 2nd param, etc.: ecx, edx, r8d - r11d
; Floating points- single or double parameters:  xmm0 to xmm5


section .data
msg db "Hello World ASM HERE", 13, 10, 0
const255 dd 255.0
z dd 0.0
section .text
bits 64
default rel   ; to handle address relocation



global imgCvtGrayFloatToInt
extern printf

imgCvtGrayFloatToInt:

    imul rcx, rdx
    imul rcx, 4
    mov r10, 0
	movss xmm1, [const255]

L1:
	mov rax, [r8]
	mov dword[z], eax
	movss xmm0, [z]
	vmulss xmm2, xmm1, xmm0
	cvtss2si ebx, xmm2
	mov dword[r9], ebx

    add r8, 4
    add r9, 4
    sub rcx, 4
    jnz L1
    ret




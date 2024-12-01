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

	mov r10, rdx
	movss xmm1, [const255]
	
L1:
	
	mov rdx, r10
	
	L2:

	; Get pointer information from matrix, using rdx as basis
	mov rax, [r8]
	imul r11, rdx, 4
	add rax, r11
	sub rax, 4 ; Decrement to account 0 based index

	mov r11, [rax]
	mov dword[z], r11d
	movss xmm0, [z]
	vmulss xmm2, xmm1, xmm0
	cvtss2si r11d, xmm2

	; Get pointer information from matrix, using rdx as basis
	mov rax, [r9] ; Pointer of row
	add rax, rdx
	dec rax       ; Decrement to account 0 based index
	mov byte[rax], r11b ; Pointer of specific column


	
	;Counter for how many in each row
	dec rdx
	jnz L2

	; Loop to next row to fill with answers
	add r9, 8
    add r8, 8
    
    dec rcx
    jnz L1

    ret



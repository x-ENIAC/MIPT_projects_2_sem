
; CALLING CONVENTION - SYSTEM V
; RDI, RSI, RDX, RCX, R8, R9, [XYZ]MM0–7

section .text

global asm_get_hash

asm_get_hash:

	crc32 edi, esi
	not edi

	mov rcx, rdx

	mov eax, edi
	sar edi, cl
	sal edi, cl

	sub eax, edi

	ret

















	

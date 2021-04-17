
; CALLING CONVENTION - SYSTEM V
; RDI, RSI, RDX, RCX, R8, R9, [XYZ]MM0–7

section .text

global asm_get_hash

asm_get_hash:
	push rbp
	mov rbp, rsp

	push r12
	push r13
	push r14
	push r15

	xor r15, r15		; r15 = hash

    mov r12, rdi		; r12 = string address
    mov r13, rsi		; r13 = length word
    mov r14, 1			; r14 = delta
    mov r10, rdx		; r10 = SIZE_HASH_TABLE
    ;mov rcx, rcx		; rcx  = DELTA_FOR_DELTA

    xor r9, r9

  loop_with_getting_hash:

  	cmp r13, 0			; if length == 0
  	je end_get_hash

	mov r9b, [r12]
	sub r9b, 96			; word[pos] - 'a' + 1
	mov rax, r9			
	mul r14				; (word[pos] - 'a' + 1) * delta
	add r14, rcx			; delta += DELTA_FOR_DELTA

	;div r10				; (word[pos] - 'a' + 1) * delta % SIZE_HASH_TABLE
	mov rdx, rax
	sar rax, 17
	sal rax, 17
	sub rdx, rax
	mov rax, rdx

	add r15, rax		; hash = hash + (word[pos] - 'a' + 1) * delta % SIZE_HASH_TABLE
	mov rax, r15
	;div r10	
	mov rdx, rax			
	sar rax, 17
	sal rax, 17
	sub rdx, rax
	mov rax, rdx

	mov r15, rax		; hash = (hash + (word[pos] - 'a' + 1) * delta % SIZE_HASH_TABLE) % SIZE_HASH_TABLE
	inc r12
	dec r13

	jmp loop_with_getting_hash

  end_get_hash:

  	mov rax, r15

  	pop r15
  	pop r14
  	pop r13
  	pop r12

	pop rbp

	ret
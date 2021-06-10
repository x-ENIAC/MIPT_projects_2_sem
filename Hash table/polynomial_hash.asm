
; CALLING CONVENTION - SYSTEM V
; RDI, RSI, RDX, RCX, R8, R9, [XYZ]MM0–7

section .text

global asm_get_hash

MAX_POW_TWO_LESS_THAN_SIZE_HASH_TABLE equ 13

asm_get_hash:

	xor r11, r11		; r11 = hash

    ;mov r12, rdi		; r12 = string address
    ;mov rsi, rsi		; r13 = length word
    mov r8, 1			; r8 = delta
    mov r10, rdx		; r10 = SIZE_HASH_TABLE
    ;mov rcx, rcx		; rcx  = DELTA_FOR_DELTA

    xor r9, r9

  loop_with_getting_hash:

  	cmp rsi, 0			; if length == 0
  	je end_get_hash

	mov r9b, [rdi]
	sub r9b, 'a'		; word[pos] - 'a' + 1
	add r9b, 1
	mov rax, r9			
	mul r8				; (word[pos] - 'a' + 1) * delta
	add r8, rcx		; delta += DELTA_FOR_DELTA

	mov rdx, rax		; (word[pos] - 'a' + 1) * delta % SIZE_HASH_TABLE
	sar rax, MAX_POW_TWO_LESS_THAN_SIZE_HASH_TABLE
	sal rax, MAX_POW_TWO_LESS_THAN_SIZE_HASH_TABLE
	sub rdx, rax
	mov rax, rdx

	add r11, rax		; hash = hash + (word[pos] - 'a' + 1) * delta % SIZE_HASH_TABLE
	mov rax, r11
	mov rdx, rax			
	sar rax, MAX_POW_TWO_LESS_THAN_SIZE_HASH_TABLE
	sal rax, MAX_POW_TWO_LESS_THAN_SIZE_HASH_TABLE
	sub rdx, rax
	mov rax, rdx

	mov r11, rax		; hash = (hash + (word[pos] - 'a' + 1) * delta % SIZE_HASH_TABLE) % SIZE_HASH_TABLE
	inc rdi
	dec rsi

	jmp loop_with_getting_hash

  end_get_hash:

  	mov rax, r11

	ret

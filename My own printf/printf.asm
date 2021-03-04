
; nasm -f elf64 -l printf.lst printf.asm
; ld -s -o printf printf.o

; r8  - 
; r9  - number_to_itoa
; r10 - system_to_itoa
; r11 - format string
; r12 - 
; r13 - begin of the parameters
; r14 - length_number_to_itoa
; r15 - 

section .text

global _start                  

_start:    
			push '('
			push 3802
			push ')'
			push '#'
			push string_to_print
			call printf
			add rsp, 2 * 8
			;pop r13


            mov rax, 0x3C      ; exit64 (rdi)
            xor rdi, rdi
            syscall

printf:
			enter 0, 0

			mov r11, [rbp + 2 * 8]		; format string
			mov rbx, 0
			mov rcx, 0

			mov	r13, 3 * 8			; begin of the parameters

			
global_handler:

			mov rax, r11
			add rax, rcx

			mov dl, byte [rax]

			cmp dl, 0x00
			je end_printf

			cmp dl, '%'
			je percent_handler			

		    inc rcx				; length message

            jmp global_handler	

percent_handler:

			push rcx
			push rdx
			push r11
			push rax

			mov rax, 0x01       ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1          ; stdout
            mov rsi, r11
            mov rdx, rcx   		; strlen 
            syscall	

            pop rax
            pop r11
            pop rdx
            pop rcx

    		add r11, rcx
    		inc r11

    		xor rcx, rcx

    		inc rax
    		mov dh, byte [rax]

    		cmp dh, 'c'
    		je char_handler

    		cmp dh, '%'
    		je printf_procent

    		cmp dh, 'd'
    		je decimal_handler    		


    		jmp global_handler

char_handler:

			push rcx
			push r11
			
			mov rax, 0x01       ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1          ; stdout
            mov rsi, rbp
            add rsi, r13
            mov rdx, 1    		; strlen 
            syscall	 

            pop r11
            pop rcx
            inc r11

            add r13, 1 * 8

    		xor rcx, rcx

            jmp global_handler  	

printf_procent:
			push r11
			
			mov rax, 0x01       ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1          ; stdout
            mov rsi, r11
            mov rdx, 1    		; strlen 
            syscall	 

            pop r11
            inc r11

    		xor rcx, rcx

            jmp global_handler  

decimal_handler:

			mov r9, rbp
			add r9, r13
			add r13, 1 * 8

			mov r10, 10
			mov rax, [r9]
			call itoa

			mov r8, rcx
			mov r9, r11

			mov rsi, rsp		

	print_numbres:
			mov al, byte [rsi]
			cmp al, '$'
			je end_print_decimal

			mov rax, 0x01       ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1          ; stdout
            mov rdx, 1    		; strlen 
            syscall	 

            pop r14

            add rsi, 1 * 8
            ;inc rsi
            jmp print_numbres

    end_print_decimal:
    		pop r14				; delete '$'

            mov r11, r9
            mov rcx, r8

            inc r11

    		xor rcx, rcx
    		;add rsp, r14 * 8

            jmp global_handler  


end_printf:
			
			mov rax, 0x01       ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1          ; stdout
            mov rsi, r11
            mov rdx, rcx    		; strlen 
            syscall	 

            leave

            ret


itoa:
			pop r8

			xor r14, r14
			push '$'

			cmp r9, 0x00
			jne itoa_loop

			push '0'
			inc r14
			jmp end_itoa_func

		itoa_loop:	

			cmp rax, 0x00
			je end_itoa_func
			
			mov rdx, 0x00
			div r10
			add rdx, '0'

			cmp rdx, '9'
			jbe is_a_digit

			add rdx, 'A' - '9' - 1
			
		is_a_digit:

			push rdx
			inc r14
			jmp itoa_loop

		end_itoa_func:			

			push r8
	 
			ret         

            
section     .data
            
string_to_print	db "L%cOL%cHel%%lo%d, %cworld!%%", 0x00

null_symbol			db 0

char_symbol			db 'c'
string_symbol		db 's'
percent_symbol		db '%'

binary_symbol		db 'b'
octal_symbol		db 'o'
decimal_symbol		db 'd'
hexadecimal_symbol	db 'x'
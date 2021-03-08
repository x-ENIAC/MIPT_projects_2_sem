
; nasm -f elf64 -l printf.lst printf.asm
; ld -s -o printf printf.o

; r8  - 
; r9  - address number to itoa
; r10 - system to itoa
; r11 - format string
; r12 - 
; r13 - begin of the parametersmy_asm_printf
; r14 - length number to itoa
; r15 - 

section .text

global _start 
global my_asm_printf     
global my_sum            
extern add_numbers

_start:    
;			push 10
;			push 15
;			inc rdi
;    		call add_numbers
;    		add rsp, 2 * 8
;    		
;    		push rax
;    		push str2
;    		call my_asm_printf
    		;add rsp, 2 * 8	

    		push 3802
    		push 4095
    		push 2021
    		push 8
    		push 1000000
    		push str_cats
			push 'I'		
			push test_string
			call my_asm_printf
			add rsp, 3 * 8
			pop r13


            mov rax, 0x3C      ; exit64 (rdi)
            xor rdi, rdi
            syscall

my_sum:
    mov rax, rdi
    add rax, rsi

    ;push 28
    ;push 82				
    ;call add_numbers
    ;add rsp, 1 * 8

	ret

my_asm_printf:
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
			je end_my_asm_printf

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
    		xor rdx, rdx
    		mov dl, byte [rax]
    		mov rbx, jump_table
    		shl rdx, 3
    		add rbx, rdx

    		xor rdx, rdx
    		mov dl, byte [rax]
    		jmp [rbx]

    		;cmp dh, 'c'
    		;je char_handler

    		;cmp dh, 's'
    		;je string_handler     		

    		;cmp dh, '%'
    		;je my_asm_printf_procent

    		; you go next only if you want to print number!

    		;cmp dh, 'b'
    		;je binary_handler    	

    		;cmp dh, 'o'
    		;je octal_handler

    		;cmp dh, 'd'
    		;je decimal_handler  

    		;cmp dh, 'x'
    		;je hexadecimal_handler  


    		jmp global_handler


string_handler:
			
			push rbx
			push rcx

			mov rdi, [rbp + r13]
			call get_length_string
			mov rdx, rax

			pop rcx
			pop rbx

			push rcx
			push r11
			
			mov rax, 0x01       ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1          ; stdout
            mov rsi, [rbp + r13]	; address of string
            ;mov rdx, 1    		; strlen 
            syscall	 

            pop r11
            pop rcx
            inc r11

            add r13, 1 * 8

    		xor rcx, rcx

            jmp global_handler  


get_length_string:

			mov rbx, rdi

			xor rax, rax
			mov rcx, 0xffffffff

			repne scasb 

			sub rdi, rbx
			mov rax, rdi

			ret           

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


my_asm_printf_procent:
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

binary_handler:

			mov r10, 2	
			jmp numbers_handler	   

octal_handler:

			mov r10, 8	
			jmp numbers_handler				         

decimal_handler:

			mov r10, 10	
			jmp numbers_handler

hexadecimal_handler:

			mov r10, 16	
			jmp numbers_handler					



numbers_handler:

			mov r9, rbp
			add r9, r13
			add r13, 1 * 8

			mov rax, [r9]
			call itoa 			; r10 = system

			mov r8, rcx
			mov r9, r11

			mov rsi, rsp		

	print_numbers_from_stack:

			mov al, byte [rsi]
			cmp al, '$'
			je end_print_numbers_from_stack

			mov rax, 0x01       ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1          ; stdout
            mov rdx, 1    		; strlen 
            syscall	 

            pop r14

            add rsi, 1 * 8
            ;inc rsi
            jmp print_numbers_from_stack

    end_print_numbers_from_stack:

    		pop r14				; delete '$'

            mov r11, r9
            mov rcx, r8

            inc r11

    		xor rcx, rcx

            jmp global_handler  


end_my_asm_printf:
			
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
            
test_string			db "Well, %c love %s on %d%% (today March %o, %b year. Press %x me. Be happy and have a %x!", 0x00
str_cats			db "cats", 0x00

null_symbol			db 0

char_symbol			db 'c'
string_symbol		db 's'
percent_symbol		db '%'

binary_symbol		db 'b'
octal_symbol		db 'o'
decimal_symbol		db 'd'
hexadecimal_symbol	db 'x'

jump_table:	
			times ('%')				dq char_handler
									dq my_asm_printf_procent

			times ('b' - '%' - 1)	dq char_handler
									dq binary_handler
									dq char_handler
									dq decimal_handler

			times ('o' - 'd' - 1)	dq char_handler
									dq octal_handler

			times ('s' - 'o' - 1)	dq char_handler
									dq string_handler

			times ('x' - 's' - 1)	dq char_handler
									dq hexadecimal_handler

			times (256 - 'x' - 1)	dq char_handler
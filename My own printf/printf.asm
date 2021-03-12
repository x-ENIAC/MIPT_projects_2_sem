
; nasm -f elf64 -l printf.lst printf.asm
; ld -s -o printf printf.o

; r8  - 
; r9  - address number to itoa
; r10 - system to itoa
; r11 - format string
; r12 - 
; r13 - begin of the parametersmy_asm_printf
; r14 - length number to itoa
; r15 - rbx

section .text

;global _start 
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

    		;push 3802
    		;push 4095
    		;push 2021
    		;push 8
    		;push 1000000
    		;push str_cats
			;push 'I'		
			;push test_string
			;call my_asm_printf
			;add rsp, 8 * 8
			;pop r13


            ;mov rax, 0x3C      ; exit64 (rdi)
            ;xor rdi, rdi
            ;syscall

my_sum:

   			mov rax, rdi
    		add rax, rsi

			ret

my_asm_printf:

			push rbp
			mov rbp, rsp

			push rsp
			push rbp
			push rbx
			push r12
			push r13
			push r14
			push r15


			;mov r14, [rdi]
			;mov r15, [rdi + 1]
			;mov r8, [rdi + 8]

			mov r11, rdi ;[rbp + 2 * 8]		; format string
			mov r15, rbx
			;mov rbx, 0
			;mov rcx, 0

			push r9
			push r8
			push rcx
			push rdx
			push rsi

			mov r14, rbp				; for save rbp without stack
			mov rbp, rsp			

			;mov	r13, 3 * 8			; begin of the parameters
			xor r13, r13
			xor rcx, rcx				; counter of symbols without %
			xor rdx, rdx				; for take symbols from the format line
			xor r12, r12				; counter of args

			
global_handler:

			mov rbx, r15
			call is_take_params_from_stack

			mov rax, r11
			add rax, rcx

			mov dl, byte [rax]

			cmp dl, 0x00
			je end_my_asm_printf

			cmp dl, '%'
			je percent_handler			

		    inc rcx				; length message

            jmp global_handler	

is_take_params_from_stack:

			cmp r13, count_regs_with_args * 8
			jne take_params_from_regs

			add r13, (2 + count_must_save_regs) * 8	; in stack are saving
													; rsp, rbp, rbx, r12-r15
													; + ret address

		take_params_from_regs:

			ret


percent_handler:

			inc r12

			push rcx
			push rdx
			push r11
			push rax
			push rsi

			mov rax, 0x01       ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1          ; stdout
            mov rsi, r11
            mov rdx, rcx   		; strlen 
            syscall	

            pop rsi
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
			
			push rdi
			push rcx
			push rsi

			mov rdi, rsi	;[rbp + r13]
			call get_length_string
			mov rdx, rcx

			pop rsi
			pop rcx
			pop rdi

			push rcx
			push r11
			push rsi
			
			mov rax, 0x01       ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1          ; stdout
            mov rsi, rbp
            add rsi, r13
            mov rsi, [rsi]
            ;mov rdx, rdx    	; strlen is already in rdx
            syscall	 

            pop rsi
            pop r11
            pop rcx

            inc r11

            add r13, 1 * 8

    		xor rcx, rcx
    		dec rsi

            jmp global_handler  


get_length_string:
	
			mov rdi, rbp
			add rdi, r13

			mov rdi, [rdi]
			mov rax, rdi

			xor rcx, rcx

		find_zero_into_line:
			mov rax, rdi
			add rax, rcx

			cmp byte [rax], 0
			je end_find_zero_into_line

			inc rcx

			jmp find_zero_into_line

		end_find_zero_into_line:

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
			dec r12
			
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
			inc rsi
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

			pop rsi
			pop rdx
			pop rcx
			pop r8
			pop r9

			pop r15
			pop r14
			pop r13
			pop r12
			pop rbx
			pop rbp
			pop rsp

            pop rbp


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
            
test_string			db "Well, %c love %s on %d%% (today March %o, %b year. Press %x me. Be happy and have a %x!)", 0x00
str_cats			db "cats", 0x00

null_symbol			db 0

char_symbol			db 'c'
string_symbol		db 's'
percent_symbol		db '%'

binary_symbol		db 'b'
octal_symbol		db 'o'
decimal_symbol		db 'd'
hexadecimal_symbol	db 'x'

count_must_save_regs	equ 7
count_regs_with_args	equ 5

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

; nasm -f elf64 -l printf.lst printf.asm
; ld -s -o printf printf.o

section .text

global _start                  

_start:    
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

            add r13, 8

    		xor rcx, rcx

            jmp global_handler  		

end_printf:
			
			mov rax, 0x01       ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1          ; stdout
            mov rsi, r11
            mov rdx, rcx    		; strlen 
            syscall	 

            leave

            ret

            
section     .data
            
string_to_print	db "L%cOL%cHello, world!", 0x00

null_symbol			db 0

char_symbol			db 'c'
string_symbol		db 's'
percent_symbol		db '%'

binary_symbol		db 'b'
octal_symbol		db 'o'
decimal_symbol		db 'd'
hexadecimal_symbol	db 'x'
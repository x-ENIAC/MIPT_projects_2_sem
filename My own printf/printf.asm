
; nasm -f elf64 -l printf.lst printf.asm
; ld -s -o printf printf.o

section .text

global _start                  

_start:    
			push string_to_print
			call printf
			;add rsp, 1
			;pop r13


            mov rax, 0x3C      ; exit64 (rdi)
            xor rdi, rdi
            syscall

printf:
			pop r12				; address of return

			mov r11, rbp
			mov rbp, rsp

			pop	rbx

		    mov rax, 0x01      ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1         ; stdout
            mov rsi, rbx
            mov rdx, 1    ; strlen (Msg)
            syscall		


            push r12

            nop

            ret

            
section     .data
            
string_to_print	db "Hello, world!", 0

char_symbol			db 'c'
string_symbol		db 's'
percent_symbol		db '%'

binary_symbol		db 'b'
octal_symbol		db 'o'
decimal_symbol		db 'd'
hexadecimal_symbol	db 'x'
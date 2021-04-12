#include <stdio.h>
#include <stdlib.h>

char* _strcpy(char* memory, const char* string_for_copy) {
	char* result = memory;

	__asm__ ( "   mov rsi, %0	; "
			  "   mov rdi, %1	; "
			  "n: mov al, [rsi]	; "
			  "	  mov [rdi], al	; "
			  "   or al, 0		; "
			  "	  jz e 			; "
			  "	  inc rsi		; "
			  "   inc rdi		; "
			  "	  jmp n 		; "
			  " e:				; "
			  :: "m"(string_for_copy), "m"(memory)	// переменные, которые доступны вставке
			  :"%rdi", "%rsi", "%rax"	// список разрушаемых, затрагиваемых регистров
		);

	return result;
}

int main() {

    char* copied = (char*)calloc(64, sizeof(char));
    copied = _strcpy(copied, "Hello, world!");
    printf("%s\n", copied);

    free(copied);

    return 0;
}
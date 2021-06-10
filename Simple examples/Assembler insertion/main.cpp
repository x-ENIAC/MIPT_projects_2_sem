#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

unsigned long long get_hash_word(const char* word, const int length_word) {
	if(!word || length_word < 0)
		return 1;

	unsigned long long hash = 0, delta = 1;

	for(int pos = 0; pos < length_word; ++pos) {		
		hash = hash + (word[pos] - 'a' + 1) * delta;
		delta += 1000;
		//printf("%c - %lld (%d, %d)\n", word[pos], hash, word[pos], word[pos] - 'a' + 1);
	}

	return hash;
}

void get_hash(const char* my_string, const int length) {

	unsigned long long hash = 0;
	unsigned long long delta = 1;

	asm ("mov r12, %1		; " // r12 = my_string
		 "xor r10, r10		; " // r10 = hash
		 "mov rbx, %2		; "	// rbx = delta
		 "mov rcx, 4		; " // rcx = length
		 "xor r11, r11		; "

	  "get_hash:			; "
		 "cmp rcx, 0		; "
		 "je end_get_hash	; "
		 "mov r11b, [r12]	; "
		 "sub r11b, 96		; "
		 "mov rax, r11		; "
		 "mul rbx			; "
		 "add rbx, 1000		; "
		 "add r10, rax		; "
		 "inc r12			; "
		 "dec rcx			; "
		 "jmp get_hash 		; "

	  "end_get_hash:		; "
	  	 "mov %0, r10		; "
	    : "=r" (hash)										// something will be written to these variables 
	    : "r" (my_string), "r"(delta), "r"(length)			// something will be read from these variables 
	    : "rax", "rbx", "rcx", "r10", "r11", "r12"			// trash-list
	);

	printf("%lld\n", hash);

	return;
}

int main() {

    get_hash("heir", 4);

    printf("%lld\n", get_hash_word("heir", 4));

    return 0;
}

// g++ c_code.cpp -c -o ./c_code.o
// nasm -f elf64 asm_code.asm
// ld asm_code.o c_code.o -o ./a.out
// ./a.out

//// g++ sum.cpp -c -o ./sum.o; nasm -f elf64 printf.asm; ld printf.o sum.o -o ./a.out; ./a.out



#include <stdio.h>

// nasm -f elf64 -o printf.o printf.asm  ; g++ -c sum.cpp -o sum.o  ; g++ sum.o printf.o  <--- !!!

extern "C" {
    void my_asm_printf(const char*, ...);

    int my_sum(int a, int b);

    int add_numbers(int number1, int number2) {
        int answer = number1 + number2;
        //my_asm_printf("%d", answer);
        return answer;
    }
}


int main() {
	//my_asm_printf(".!%d.Hey, %s! Today we are going to eat %d pizza%c. %o %% %x, he-he", 5555, "dear", 2, "s", 8, 3802);
	//my_asm_printf("Well, %c love %s on %d%% (today March %o, %b year. Press %x me. Be happy and have a %x!)", 'I', "cats", 1000000, 8, 2021, 4095, 3802);
	my_asm_printf("Hahaha. %c%c%c%c%c%c", 'h', 'e', 'l', 'p', ',', 'm');

	//printf("Sum of 5 and 15 is %d\n", my_sum(5, 15));

	return 0;
}

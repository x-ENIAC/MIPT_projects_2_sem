

// nasm -f elf64 -o printf.o printf.asm  ; g++ -c sum.cpp -o sum.o  ; g++ sum.o printf.o  <--- !!!

extern "C" {
    void my_asm_printf(const char*, ...);

    int my_sum(int a, int b);

    int add_numbers(int number1, int number2) {
        int answer = number1 + number2;
        return answer;
    }
}


int main() {
	//my_asm_printf("5 + 10 = %d. I'm %s :) (maybe...)", my_sum(5, 10), "clever");
	my_asm_printf(". !%d.Hey, %s! Today we are going to eat %d pizza%c. %o%% %x, he-he", 5555, "dear", 2, 's', 8, 3802);
	my_asm_printf(", Well, %c love %s on %d%% (today March %o, %b year. Press %x me. Be happy and have a %x!)", 'I', "cats", 1000000, 8, 2021, 4095, 3802);
	
	//printf("\n");
	
	my_asm_printf(", and I %s %x %d%%%c%b", "looove", 3802, 100, 33, 255);
	
	my_asm_printf("Hahaha. %c%c%c%c%c%c%c", 'h', 'e', 'l', 'p', ',', 'm', 'e');
	my_asm_printf("Hahaha. %c%c%c%c%c", 'h', 'e', 'l', 'p', ',');

	//printf("Sum of 5 and 15 is %d\n", my_sum(5, 15));

	return 0;
}

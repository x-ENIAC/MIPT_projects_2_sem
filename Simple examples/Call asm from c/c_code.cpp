// nasm -f elf64 <name_asm_file>.asm
// gcc -Wall <name_c_file>.cpp <name_asm_file>.o
// ./a.out

#include <stdio.h>

extern "C" int add_numbers(int, int);

int main() {
    int number1 = 0, number2 = 0;
    scanf("%d%d", &number1, &number2);

    int sum = add_numbers(number1, number2);
    printf("%d + %d = %d\n", number1, number2, sum);
    return 0;
}

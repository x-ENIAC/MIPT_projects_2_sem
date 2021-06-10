all:
	nasm -f elf64 asm_code.asm
	gcc -Wall c_code.cpp asm_code.o

run: all
	./a.out

debug: all
	valgrind ./a.out

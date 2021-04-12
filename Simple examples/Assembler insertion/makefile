all:
	g++ main.cpp -masm=intel

run: all
	./a.out

debug: all
	valgrind ./a.out

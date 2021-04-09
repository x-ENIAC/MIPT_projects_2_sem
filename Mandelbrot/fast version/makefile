all:
	g++ -c -g -O1 -mavx2 main.cpp mandelbrot.cpp

run: all
	a.exe

debug: all
	valgrind a.exe
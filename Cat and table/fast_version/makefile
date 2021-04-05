all:
	g++ -c -g -O1 -march=native main.cpp
	g++ main.o  -mavx2 -o cat_on_table image_processing.cpp -lsfml-graphics -lsfml-window -lsfml-system

run: all
	./cat_on_table

debug: all
	valgrind ./cat_on_table

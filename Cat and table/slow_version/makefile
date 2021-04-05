all:
	g++ -c main.cpp
	g++ main.o -o sfml_app image_processing.cpp -lsfml-graphics -lsfml-window -lsfml-system

run:	all
	./sfml_app
	
debug:	all
	valgrind ./sfml_app

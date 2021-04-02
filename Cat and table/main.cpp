// g++ -c main.cpp
// g++ main.o -o sfml_app image_processing.cpp -lsfml-graphics -lsfml-window -lsfml-system


#include <stdio.h>
#include <stdlib.h>
#include "image_processing.h"
#include <SFML/Graphics.hpp>


#define CHECK_STATUS									\
	if(status != ALL_IS_OKEY) {							\
		printf("Bad; status = %d\n", status);			\
	}

#define CHECK_STATUS_AND_RETURN_IF_NOT_OKEY				\
	if(status != ALL_IS_OKEY) {							\
		printf("Bad; status = %d\n", status);			\
		return status;									\
	}	

int main() {
	Statuses_type status = ALL_IS_OKEY;

	Screen_type* background_picture = screen_new("Table.bmp");
	CHECK_STATUS_AND_RETURN_IF_NOT_OKEY

	Screen_type* foreground_picture = screen_new("AskhatCat.bmp");
	CHECK_STATUS_AND_RETURN_IF_NOT_OKEY


	status = start_overlaying_pictures(background_picture, foreground_picture);
	CHECK_STATUS


	return ALL_IS_OKEY;
}
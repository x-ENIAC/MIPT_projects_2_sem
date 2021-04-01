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

Screen_type* screen_new(const char* picture_name)	 {
	Screen_type* new_screen = (Screen_type*)calloc(1, sizeof(Screen_type));

	if(!new_screen)
		return NULL;

	Statuses_type status = screen_construct(new_screen, picture_name);

	if(status != ALL_IS_OKEY) {
		free(new_screen);
		return NULL;
	}

	return new_screen;
}

Statuses_type screen_construct(Screen_type* screen, const char* picture_name) {
	if(!screen)
		return BAD_POINTERS;

	sf::Image image;
	image.loadFromFile(picture_name);

	sf::Vector2u image_size = image.getSize();

	screen->wigth_screen  = image_size.x;
	screen->height_screen = image_size.y;

	screen->pixels = (Colour**)calloc(screen->height_screen, sizeof(Colour*));

	if(!screen->pixels)
		return BAD_POINTERS;

	for(int i = 0; i < screen->wigth_screen; ++i) {
		screen->pixels[i] = (Colour*)calloc(screen->wigth_screen, sizeof(Colour));
		if(!screen->pixels[i])
			return BAD_POINTERS;
	}

	Statuses_type status = ALL_IS_OKEY;

	for(int i = 0; i < screen->height_screen; ++i) {
		for(int j = 0; j < screen->wigth_screen; ++j) {
			sf::Color color_now_pixel = image.getPixel(j, i);
			set_pixel_color(screen->pixels, i, j, color_now_pixel);
			CHECK_STATUS_AND_RETURN_IF_NOT_OKEY
		}
	}

	return ALL_IS_OKEY;

}

Statuses_type set_pixel_color(Colour** pixels, const int line, const int column, sf::Color color_now_pixel) {
	if(!pixels || !pixels[line])
		return BAD_POINTERS;

	pixels[line][column].red   = color_now_pixel.r;
	pixels[line][column].green = color_now_pixel.g;
	pixels[line][column].blue  = color_now_pixel.b;
	pixels[line][column].alpha = color_now_pixel.a;

	return ALL_IS_OKEY;
}
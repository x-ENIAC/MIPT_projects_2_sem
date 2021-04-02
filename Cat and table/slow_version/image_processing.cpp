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

	sf::Image image = {};
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

Statuses_type screen_delete(Screen_type* screen) {
	if(!screen || !screen->pixels)
		return ALL_IS_OKEY;

	for(int i = 0; i < screen->height_screen; ++i)
		free(screen->pixels[i]);

	free(screen->pixels);

	return ALL_IS_OKEY;

}


Statuses_type start_overlaying_pictures(Screen_type* background_picture, Screen_type* foreground_picture) {
	if(!background_picture || !foreground_picture) 
		return BAD_POINTERS;

	if(!is_correct_pictures_size(background_picture, foreground_picture))
		return BAD_PICTURES_SIZE;

	sf::Sprite sprite = {};
	sf::Texture texture = {};

	if(!texture.create(background_picture->wigth_screen, background_picture->height_screen)) 
		return TEXTURE_NOT_CREATED;

	sprite.setTexture(texture);

	printf("begin overlaying pictures...\n");

	measurements(background_picture, foreground_picture);

	printf("begin show_result_images...\n");

	show_result_image(background_picture, &sprite, &texture);

	return ALL_IS_OKEY;
}

Statuses_type measurements(Screen_type* background_picture, Screen_type* foreground_picture) {
	if(!background_picture || !foreground_picture)
		return BAD_POINTERS;

	clock_t start_time = clock();

	Statuses_type status = ALL_IS_OKEY;
	for(int measurement = 0; measurement < count_repeats_in_measurements; ++measurement) {
		status = overlaying_pictures(background_picture, foreground_picture, X_OFFSET, Y_OFFSET);
		CHECK_STATUS_AND_RETURN_IF_NOT_OKEY
	}

	clock_t end_time = clock();

	printf("Time passed: %f ms\n", (end_time - start_time) / (float)count_repeats_in_measurements);

	return ALL_IS_OKEY;
}

Statuses_type overlaying_pictures(Screen_type* background_picture, Screen_type* foreground_picture, const int x_offset, const int y_offset) {
	if(!background_picture || !foreground_picture)
		return BAD_POINTERS;

	if(!is_correct_pictures_size(background_picture, foreground_picture))
		return BAD_PICTURES_SIZE;
	

	for(int x = 0; x < foreground_picture->height_screen; ++x) {
		for(int y = 0; y < foreground_picture->wigth_screen; ++y) {
			Colour background_pixel = background_picture->pixels[x + x_offset][y + y_offset];
			Colour foreground_pixel = foreground_picture->pixels[x][y];

			background_picture->pixels[x + x_offset][y + y_offset] = pixel_transform_on_overlay(background_pixel, foreground_pixel);
		}
	}

	return ALL_IS_OKEY;
}

bool is_correct_pictures_size(Screen_type* background_picture, Screen_type* foreground_picture) {
	if(!background_picture || !foreground_picture)
		return false;

	if(foreground_picture->wigth_screen  > background_picture->wigth_screen ||
	   background_picture->height_screen > background_picture->height_screen)
		return false;

	return true;
}

Colour pixel_transform_on_overlay(const Colour background_pixel, const Colour foreground_pixel) {
	Colour result_pixel = {0, 0, 0, 0};

	float alpha = (float)foreground_pixel.alpha;

	result_pixel.red   = ((int)((float)foreground_pixel.red   * alpha + (float)background_pixel.red   * (MAX_COLOUR - alpha)) >> 8);
	result_pixel.green = ((int)((float)foreground_pixel.green * alpha + (float)background_pixel.green * (MAX_COLOUR - alpha)) >> 8);
	result_pixel.blue  = ((int)((float)foreground_pixel.blue  * alpha + (float)background_pixel.blue  * (MAX_COLOUR - alpha)) >> 8);

	result_pixel.alpha = MAX_COLOUR - 1;

	return result_pixel;
}

void show_result_image(Screen_type* background_picture, sf::Sprite* sprite, sf::Texture* texture) {
	sf::Image result_image = {};
	result_image.create(background_picture->wigth_screen, background_picture->height_screen);

	for(int x = 0; x < background_picture->height_screen; ++x) {
		for(int y = 0; y < background_picture->wigth_screen; ++y) {
			sf::Color now_color(background_picture->pixels[x][y].red,
								background_picture->pixels[x][y].green,
								background_picture->pixels[x][y].blue,
								background_picture->pixels[x][y].alpha);

			result_image.setPixel(y, x, now_color);
		}
	}

	texture->update(result_image);

	display_picture(*sprite, background_picture->wigth_screen, background_picture->height_screen);
}

void display_picture(sf::Sprite sprite, const int wigth_screen, const int height_screen) {
	sf::RenderWindow window(sf::VideoMode(wigth_screen, height_screen), "Cat on table!");

	while(window.isOpen()) {
		sf::Event event = {};
		while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
	}
}
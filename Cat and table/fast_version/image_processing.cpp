#include <stdio.h>
#include "image_processing.h"

<<<<<<< HEAD
#define DRAWING

=======
>>>>>>> 3e2b7b1cb6a363b52cce91b6d9f20acc6fa79cb3
#define CHECK_STATUS													\
	if(status != ALL_IS_OKEY) {											\
		printf("Bad; status = %s\n", TEXT_STATUSES[status]);			\
	}

#define CHECK_STATUS_AND_RETURN_IF_NOT_OKEY								\
	if(status != ALL_IS_OKEY) {											\
		printf("Bad; status = %s\n", TEXT_STATUSES[status]);			\
		return status;													\
	}	

Screen_type* screen_new(const char* picture_name)	 {
	Screen_type* new_screen = (Screen_type*)calloc(1, sizeof(Screen_type));

	if(!new_screen || !picture_name) {
		printf("Bad; status = %s\n", TEXT_STATUSES[BAD_POINTERS]);
		return NULL;
	}

	Statuses_type status = screen_construct(new_screen, picture_name);

	if(status != ALL_IS_OKEY) {
		printf("Bad; status = %s\n", TEXT_STATUSES[status]);
		free(new_screen);
		return NULL;
	}

	return new_screen;
}

Statuses_type screen_construct(Screen_type* screen, const char* picture_name) {
	if(!screen || !picture_name)
		return BAD_POINTERS;

	sf::Image image = {};
	image.loadFromFile(picture_name);

	sf::Vector2u image_size = image.getSize();

	screen->wigth_screen  = image_size.x;
	screen->height_screen = image_size.y;

	screen->pixels = (Colour*)calloc(screen->height_screen * screen->wigth_screen, sizeof(Colour));

	if(!screen->pixels)
		return BAD_POINTERS;

	Statuses_type status = ALL_IS_OKEY;

	for(int x_coordinate = 0; x_coordinate < screen->height_screen; ++x_coordinate) {
		for(int y_coordinate = 0; y_coordinate < screen->wigth_screen; ++y_coordinate) {
			sf::Color color_now_pixel = image.getPixel(y_coordinate, x_coordinate);

			status = set_pixel_color(screen, x_coordinate, y_coordinate, color_now_pixel);
			CHECK_STATUS_AND_RETURN_IF_NOT_OKEY
		}
	}

	return status;
}

inline Statuses_type set_pixel_color(Screen_type* screen, const int line, const int column, const sf::Color color_now_pixel) {
	if(!screen || !screen->pixels)
		return BAD_POINTERS;

	screen->pixels[line * screen->wigth_screen + column].red   = color_now_pixel.r;
	screen->pixels[line * screen->wigth_screen + column].green = color_now_pixel.g;
	screen->pixels[line * screen->wigth_screen + column].blue  = color_now_pixel.b;
	screen->pixels[line * screen->wigth_screen + column].alpha = color_now_pixel.a;

	return ALL_IS_OKEY;
}

Statuses_type screen_delete(Screen_type* screen) {
	if(!screen || !screen->pixels)
		return ALL_IS_OKEY;

	free(screen->pixels);

	return ALL_IS_OKEY;
}

Statuses_type start_overlaying_pictures(Screen_type* background_picture, Screen_type* foreground_picture) {
	if(!background_picture || !foreground_picture) 
		return BAD_POINTERS;

	if(!is_correct_pictures_size(background_picture, foreground_picture))
		return BAD_PICTURES_SIZE;

	sf::Sprite  sprite = {};
	sf::Texture texture = {};

	if(!texture.create(background_picture->wigth_screen, background_picture->height_screen)) 
		return TEXTURE_NOT_CREATED;

	sprite.setTexture(texture);

	Statuses_type status = ALL_IS_OKEY;

	status = measurements(background_picture, foreground_picture);
	CHECK_STATUS_AND_RETURN_IF_NOT_OKEY

<<<<<<< HEAD
	#ifdef DRAWING
	status = show_result_image(background_picture, &sprite, &texture);
	CHECK_STATUS_AND_RETURN_IF_NOT_OKEY
	#endif
=======
	//status = show_result_image(background_picture, &sprite, &texture);
	//CHECK_STATUS_AND_RETURN_IF_NOT_OKEY
>>>>>>> 3e2b7b1cb6a363b52cce91b6d9f20acc6fa79cb3

	return status;
}

inline Statuses_type measurements(Screen_type* background_picture, Screen_type* foreground_picture) {
	if(!background_picture || !foreground_picture)
		return BAD_POINTERS;

	clock_t start_time = clock();

	Statuses_type status = ALL_IS_OKEY;

	for(int measurement = 0; measurement < COUNT_REPEATS_IN_MEASUREMENTS; ++measurement) {
		status = overlaying_pictures(background_picture, foreground_picture, Y_OFFSET, X_OFFSET);
		CHECK_STATUS_AND_RETURN_IF_NOT_OKEY
	}

	clock_t end_time = clock();

	printf("Time passed: %f ms\n", (end_time - start_time) / (float)COUNT_REPEATS_IN_MEASUREMENTS);

	return status;
}

inline Statuses_type overlaying_pictures(Screen_type* background_picture, Screen_type* foreground_picture, const int y_offset, const int x_offset) {
	if(!background_picture || !foreground_picture)
		return BAD_POINTERS;

	if(!is_correct_pictures_size(background_picture, foreground_picture) || x_offset < 0 || y_offset < 0)
		return BAD_PICTURES_SIZE;	

	int y_background = y_offset;
	int x_background = x_offset;	

	if (x_background >= background_picture->wigth_screen)
		x_background = background_picture->wigth_screen - x_background;

	if (y_background >= background_picture->height_screen)
		y_background = background_picture->height_screen - y_background;	

	for(int y_foreground = 0; y_foreground < foreground_picture->height_screen; ++y_foreground) {
		x_background = x_offset;

		for(int x_foreground = 0; x_foreground + PACK - 1 < foreground_picture->wigth_screen; x_foreground += PACK) {

			__m128i background_pixels = _mm_loadu_si128((__m128i*)(background_picture->pixels + y_background * background_picture->wigth_screen + x_background));
			__m128i foreground_pixels = _mm_loadu_si128((__m128i*)(foreground_picture->pixels + y_foreground * foreground_picture->wigth_screen + x_foreground));

			__m128i lower_background = _mm_shuffle_epi8(background_pixels, LOWER_MASK);
			__m128i upper_background = _mm_shuffle_epi8(background_pixels, UPPER_MASK);

			__m128i lower_foreground = _mm_shuffle_epi8(foreground_pixels, LOWER_MASK);
			__m128i upper_foreground = _mm_shuffle_epi8(foreground_pixels, UPPER_MASK);	

			__m128i alpha_lower = _mm_shuffle_epi8(foreground_pixels, LOWER_MASK_FOR_ALPHA);
			__m128i alpha_upper = _mm_shuffle_epi8(foreground_pixels, UPPER_MASK_FOR_ALPHA);

			__m128i inverse_lower_mask_for_alpha = _mm_sub_epi16(CONSTANT_256, alpha_lower);
			__m128i inverse_upper_mask_for_alpha = _mm_sub_epi16(CONSTANT_256, alpha_upper);

			lower_foreground = _mm_mullo_epi16(lower_foreground, alpha_lower);
			upper_foreground = _mm_mullo_epi16(upper_foreground, alpha_upper);

			lower_background = _mm_mullo_epi16(lower_background, inverse_lower_mask_for_alpha);
			upper_background = _mm_mullo_epi16(upper_background, inverse_upper_mask_for_alpha);

			lower_background = _mm_add_epi16(lower_background, lower_foreground);
			upper_background = _mm_add_epi16(upper_background, upper_foreground);

			lower_background = _mm_shuffle_epi8(lower_background, MASK_FOR_RESULT);
			upper_background = _mm_shuffle_epi8(upper_background, MASK_FOR_RESULT);

			__m128i result = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(upper_background), _mm_castsi128_ps(lower_background)));
			_mm_storeu_si128((__m128i*)(background_picture->pixels + y_background * background_picture->wigth_screen + x_background), result);   

			x_background += PACK;
		}

		++y_background;
	}

	return ALL_IS_OKEY;
}

inline bool is_correct_pictures_size(Screen_type* background_picture, Screen_type* foreground_picture) {
	if(!background_picture || !foreground_picture)
		return false;

	if(foreground_picture->wigth_screen  > background_picture->wigth_screen ||
	   foreground_picture->height_screen > background_picture->height_screen)
		return false;

	if(foreground_picture->wigth_screen < 0 || foreground_picture->height_screen < 0 ||
	   background_picture->wigth_screen < 0 || background_picture->height_screen < 0)
		return false;

	return true;
}

Statuses_type show_result_image(Screen_type* background_picture, sf::Sprite* sprite, sf::Texture* texture) {
	if(!sprite || !texture)
		return BAD_POINTERS;

	sf::Image result_image = {};
	result_image.create(background_picture->wigth_screen, background_picture->height_screen);

	for(int x_coordinate = 0; x_coordinate < background_picture->height_screen; ++x_coordinate) {
		for(int y_coordinate = 0; y_coordinate < background_picture->wigth_screen; ++y_coordinate) {
<<<<<<< HEAD

			sf::Color now_color(background_picture->pixels[x_coordinate * background_picture->wigth_screen + y_coordinate].red,
								background_picture->pixels[x_coordinate * background_picture->wigth_screen + y_coordinate].green,
								background_picture->pixels[x_coordinate * background_picture->wigth_screen + y_coordinate].blue,
								background_picture->pixels[x_coordinate * background_picture->wigth_screen + y_coordinate].alpha);

=======

			sf::Color now_color(background_picture->pixels[x_coordinate * background_picture->wigth_screen + y_coordinate].red,
								background_picture->pixels[x_coordinate * background_picture->wigth_screen + y_coordinate].green,
								background_picture->pixels[x_coordinate * background_picture->wigth_screen + y_coordinate].blue,
								background_picture->pixels[x_coordinate * background_picture->wigth_screen + y_coordinate].alpha);

>>>>>>> 3e2b7b1cb6a363b52cce91b6d9f20acc6fa79cb3
			result_image.setPixel(y_coordinate, x_coordinate, now_color);
		}
	}

	texture->update(result_image);

	display_picture(sprite, background_picture->wigth_screen, background_picture->height_screen);

	return ALL_IS_OKEY;
}

void display_picture(sf::Sprite* sprite, const int wigth_screen, const int height_screen) {
	sf::RenderWindow window(sf::VideoMode(wigth_screen, height_screen), "Cat on table!");

	while(window.isOpen()) {
		sf::Event event = {};
		while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(*sprite);
        window.display();
	}

	return;
<<<<<<< HEAD
}
=======
}
>>>>>>> 3e2b7b1cb6a363b52cce91b6d9f20acc6fa79cb3

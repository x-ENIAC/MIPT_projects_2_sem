#include <stdio.h>
//#include <stdlib.h> 
#include "image_processing.h"
#include <SFML/Graphics.hpp>
#include <emmintrin.h>
//#include <smmintrin.h>
#include <smmintrin.h>

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

	screen->pixels = (Colour*)calloc(screen->height_screen * screen->wigth_screen, sizeof(Colour));

	if(!screen->pixels)
		return BAD_POINTERS;

	Statuses_type status = ALL_IS_OKEY;

	for(int i = 0; i < screen->height_screen; ++i) {
		for(int j = 0; j < screen->wigth_screen; ++j) {
			sf::Color color_now_pixel = image.getPixel(j, i);
			set_pixel_color(screen, i, j, color_now_pixel);
			CHECK_STATUS_AND_RETURN_IF_NOT_OKEY
		}
	}

	return ALL_IS_OKEY;

}

Statuses_type set_pixel_color(Screen_type* screen, const int line, const int column, sf::Color color_now_pixel) {
	if(!screen || !screen->pixels) // || !pixels[line])
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

Statuses_type overlaying_pictures(Screen_type* background_picture, Screen_type* foreground_picture, int x_offset, int y_offset) {
	if(!background_picture || !foreground_picture)
		return BAD_POINTERS;

	if(!is_correct_pictures_size(background_picture, foreground_picture) || x_offset < 0 || y_offset < 0)
		return BAD_PICTURES_SIZE;	

	if (x_offset >= background_picture->wigth_screen)
		x_offset = background_picture->wigth_screen - x_offset;

	if (y_offset >= background_picture->height_screen)
		y_offset = background_picture->height_screen - y_offset;	

	int x_background = x_offset;
	int y_background = y_offset;

	for(int y_foreground = 0; y_foreground < foreground_picture->height_screen; ++y_foreground) {
		y_background = y_offset;
		for(int x_foreground = 0; x_foreground + 3 < foreground_picture->wigth_screen; x_foreground += 4) {

			// background [ b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0] = [r7 g7 b7 a7 | ... ]
			__m128i background_pixels = _mm_loadu_si128((__m128i*)(background_picture->pixels + x_background * background_picture->wigth_screen + y_background));

			// foreground [ f7 | f6 | f5 | f4 | f3 | f2 | f1 | f0] = [r7 g7 b7 a7 | ... ]
			__m128i foreground_pixels = _mm_loadu_si128((__m128i*)(foreground_picture->pixels + y_foreground * foreground_picture->wigth_screen + x_foreground));

			// [0 a3 0 r3 | 0 g3 0 b3 | ... | 0 a0 0 r0 | 0 g0 0 b0] - background
			__m128i background_four_lower = _mm_cvtepu8_epi16(background_pixels);

			// [0 a7 0 r7 | 0 g7 0 b7 | ... | 0 a4 0 r4 | 0 g4 0 b4] - background
			__m128i background_four_upper = _mm_cvtepu8_epi16(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(background_pixels), _mm_castsi128_ps(background_pixels))));

			// [0 a3 0 r3 | 0 g3 0 b3 | ... | 0 a0 0 r0 | 0 g0 0 b0] - foreground
			__m128i foreground_four_lower = _mm_cvtepu8_epi16(foreground_pixels);

			// [0 a7 0 r7 | 0 g7 0 b7 | ... | 0 a4 0 r4 | 0 g4 0 b4] - foreground
			__m128i foreground_four_upper = _mm_cvtepu8_epi16(_mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(foreground_pixels), _mm_castsi128_ps(foreground_pixels))));

			__m128i mask_alpha_for_upper = _mm_set_epi8(-1, 12, -1, 12,
												        -1, 12, -1, 12,
													    -1,  8, -1,  8,
											            -1,  8, -1,  8);

			__m128i mask_alpha_for_lower = _mm_set_epi8(-1, 4, -1, 4,
												        -1, 4, -1, 4,
													    -1, 0, -1, 0,
											            -1, 0, -1, 0);



			// [0 a3 0 a3 | 0 a3 0 a3 | ... | 0 a0 0 a0 | 0 a0 0 a0] - background
			__m128i alpha_lower = _mm_shuffle_epi8(foreground_pixels, mask_alpha_for_lower);

			// [0 a7 0 a7 | 0 a7 0 a7 | ... | 0 a4 0 a4 | 0 a4 0 a4] - background
			__m128i alpha_upper = _mm_shuffle_epi8(foreground_pixels, mask_alpha_for_upper);

			__m128i inverse_alpha_lower = _mm_sub_epi16(_mm_set_epi16(256, 256, 256, 256,
			 														  			  256, 256, 256, 256), alpha_lower);
			__m128i inverse_alpha_upper = _mm_sub_epi16(_mm_set_epi16(256, 256, 256, 256,
			 														  			  256, 256, 256, 256), alpha_upper);

			background_four_lower = _mm_mullo_epi16(background_four_lower, inverse_alpha_lower);
			background_four_upper = _mm_mullo_epi16(background_four_upper, inverse_alpha_upper);

			foreground_four_lower = _mm_mullo_epi16(foreground_four_lower, alpha_lower);
			foreground_four_upper = _mm_mullo_epi16(foreground_four_upper, alpha_upper);

			// [A3 a3 R3 r3 | G3 g3 B3 b3 | ... | A0 a0 R0 r0 | G0 g0 B0 b0]
			foreground_four_lower = _mm_add_epi16(background_four_lower, foreground_four_lower);

			// [A7 a7 R7 r7 | G7 g7 B7 b7 | ... | A4 a4 R4 r4 | G4 g4 B4 b4]
			foreground_four_upper = _mm_add_epi16(background_four_upper, foreground_four_upper);

			foreground_four_lower = _mm_srli_epi16(foreground_four_lower, 8);
			foreground_four_upper = _mm_srli_epi16(foreground_four_upper, 8);			


			__m128i mask_for_lowers = _mm_set_epi8(-1, -1, -1, -1,
												  -1, -1, -1, -1,
												  14, 12, 10,  8,
												   6,  4,  2,  0);

			__m128i mask_for_uppers = _mm_set_epi8(14, 12, 10, 8,
												   6,  4,  2,  0,
												  -1, -1, -1, -1,
												  -1, -1, -1, -1);			
			// [-- -- -- -- | -- -- -- -- | -- -- -- -- | -- -- -- -- | A3 R3 G3 B3 | .... | A0 R0 G0 B0 ]
			foreground_pixels = _mm_shuffle_epi8(foreground_four_lower, mask_for_uppers);
			

			foreground_four_upper = _mm_shuffle_epi8(foreground_four_upper, mask_for_uppers);


			//result_pixel = _mm_shuffle_epi8(lower_pixels, mask_for_uppers);
			foreground_pixels = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(foreground_four_upper), _mm_castsi128_ps(foreground_pixels)));

			// [A7 R7 G7 B7 | A6 R6 G6 B6 | -- -- -- -- | -- -- -- -- | A3 R3 G3 B3 | .... | A0 R0 G0 B0 ]
			//result_pixel = _mm_shuffle_epi8(upper_pixels, mask_for_uppers);
			_mm_storeu_si128((__m128i*)(background_picture->pixels + x_background * background_picture->wigth_screen + y_background), foreground_pixels);

			y_background += 4;
		}
		++x_background;
	}

	return ALL_IS_OKEY;
}

void set_4_numbers_from_array_to_m128(__m128* big_number, const int first_number, const int second_number, const int third_number, const int fourth_number) {
	if(!big_number)
		return;

	*big_number = _mm_castsi128_ps(_mm_set_epi32(first_number, second_number, third_number, fourth_number)); // [fourth | third | second | first]

	return;
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

	int alpha = 140;//(int)foreground_pixel.alpha;

	result_pixel.red   = ((int)((int)foreground_pixel.red   * alpha + (int)background_pixel.red   * (MAX_COLOUR - alpha)) >> 8); 
	result_pixel.green = ((int)((int)foreground_pixel.green * alpha + (int)background_pixel.green * (MAX_COLOUR - alpha)) >> 8);
	result_pixel.blue  = ((int)((int)foreground_pixel.blue  * alpha + (int)background_pixel.blue  * (MAX_COLOUR - alpha)) >> 8);

	result_pixel.alpha = MAX_COLOUR - 1;

	return result_pixel;
}

void show_result_image(Screen_type* background_picture, sf::Sprite* sprite, sf::Texture* texture) {
	sf::Image result_image = {};
	result_image.create(background_picture->wigth_screen, background_picture->height_screen);

	for(int x = 0; x < background_picture->height_screen; ++x) {
		for(int y = 0; y < background_picture->wigth_screen; ++y) {
			sf::Color now_color(background_picture->pixels[x * background_picture->wigth_screen + y].red,
								background_picture->pixels[x * background_picture->wigth_screen + y].green,
								background_picture->pixels[x * background_picture->wigth_screen + y].blue,
								background_picture->pixels[x * background_picture->wigth_screen + y].alpha);

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


/*
		/*(-1, 30, -1, 30,
														 -1, 30, -1, 30,
														 -1, 22, -1, 22,
														 -1, 22, -1, 22,
														 -1, 14, -1, 14,
														 -1, 14, -1, 14,
														 -1,  6, -1,  6,
														 -1,  6, -1,  6);*/



/*
			__m128 mask_for_lowers = _mm_set_epi8(-1, -1, -1, -1,
												  -1, -1, -1, -1,
												  -1, -1, -1, -1,
												  -1, -1, -1, -1,
												  31, 29, 27, 25,
												  23, 21, 19, 17,
												  15, 13, 11,  9,
												   7,  5,  3,  1);
			// [-- -- -- -- | -- -- -- -- | -- -- -- -- | -- -- -- -- | A3 R3 G3 B3 | .... | A0 R0 G0 B0 ]
			__m128 result_pixel = _mm_shuffle_epi8(lower_pixels, mask_for_lowers);


			__m128 mask_for_uppers = _mm_set_epi8(31, 29, 27, 25,
												  23, 21, 19, 17,
												  15, 13, 11,  9,
												   7,  5,  3,  1,
												  -1, -1, -1, -1,
												  -1, -1, -1, -1,
												  -1, -1, -1, -1,
												  -1, -1, -1, -1);	
*/
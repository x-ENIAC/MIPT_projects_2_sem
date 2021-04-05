#include <stdio.h>
//#include <stdlib.h> 
#include "image_processing.h"
#include <SFML/Graphics.hpp>
#include <emmintrin.h>
#include <immintrin.h>
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

    __m128i c256 = _mm_set1_epi16(256);

	for(int y_foreground = 0; y_foreground < foreground_picture->height_screen; ++y_foreground) {
		y_background = y_offset;
		for(int x_foreground = 0; x_foreground + 3 < foreground_picture->wigth_screen; x_foreground += 4) {

			__m128i background_pixels = _mm_loadu_si128((__m128i*)(background_picture->pixels + x_background * background_picture->wigth_screen + y_background));

			__m128i foreground_pixels = _mm_loadu_si128((__m128i*)(foreground_picture->pixels + y_foreground * foreground_picture->wigth_screen + x_foreground));

			__m128i lower_mask = _mm_set_epi8(0x80, 7, 0x80, 6,
											  0x80, 5, 0x80, 4,
											  0x80, 3, 0x80, 2,
											  0x80, 1, 0x80, 0);

			__m128i upper_mask = _mm_set_epi8(0x80, 15, 0x80, 14,
											  0x80, 13, 0x80, 12,
											  0x80, 11, 0x80, 10,
											  0x80,  9, 0x80,  8);			

			__m128i lower_background = _mm_shuffle_epi8(background_pixels, lower_mask);
			__m128i upper_background = _mm_shuffle_epi8(background_pixels, upper_mask);

			__m128i lower_foreground = _mm_shuffle_epi8(foreground_pixels, lower_mask);
			__m128i upper_foreground = _mm_shuffle_epi8(foreground_pixels, upper_mask);	

	
			__m128i lower_mask_for_alpha = _mm_set_epi8(-1,  4, -1,  4,
												   		-1,  4,  -1,  4,
												   		-1,  0,  -1,  0,
												   		-1,  0,  -1,  0);

			__m128i upper_mask_for_alpha = _mm_set_epi8(-1, 12, -1, 12,
												  		-1, 12, -1, 12,
												   		-1,  8,  -1,  8,
												   		-1,  8,  -1,  8);	

			__m128i alpha_lower = _mm_shuffle_epi8(foreground_pixels, lower_mask_for_alpha);
			__m128i alpha_upper = _mm_shuffle_epi8(foreground_pixels, upper_mask_for_alpha);

			__m128i inverse_lower_mask_for_alpha = _mm_sub_epi16(c256, alpha_lower);
			__m128i inverse_upper_mask_for_alpha = _mm_sub_epi16(c256, alpha_upper);

			lower_foreground = _mm_mullo_epi16(lower_foreground, alpha_lower);
			upper_foreground = _mm_mullo_epi16(upper_foreground, alpha_upper);

			lower_background = _mm_mullo_epi16(lower_background, inverse_lower_mask_for_alpha);
			upper_background = _mm_mullo_epi16(upper_background, inverse_upper_mask_for_alpha);

			lower_background = _mm_add_epi16(lower_background, lower_foreground);
			upper_background = _mm_add_epi16(upper_background, upper_foreground);

			__m128i mask_for_result = _mm_set_epi8(15, 13, 11,  9,
												    7,  5,  3,  1,
												   0x80, 0x80, 0x80, 0x80,
												   0x80, 0x80, 0x80, 0x80);

			lower_background = _mm_shuffle_epi8(lower_background, mask_for_result);
			upper_background = _mm_shuffle_epi8(upper_background, mask_for_result);

			__m128i result = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(upper_background), _mm_castsi128_ps(lower_background)));
			_mm_storeu_si128((__m128i*)(background_picture->pixels + x_background * background_picture->wigth_screen + y_background), result);

			/*__m256i fg = _mm256_cvtepu8_epi16(foreground_pixels);
            __m256i bg = _mm256_cvtepu8_epi16(background_pixels);

            __m256i lower_alpha_mask = _mm256_set_epi8(0x80, 0x80, 0x80, 0x80,
												 	   0x80, 0x80, 0x80, 0x80,
													   0x80, 0x80, 0x80, 0x80,
													   0x80, 0x80, 0x80, 0x80,
												       12, 12, 12, 12,
												        8,  8,  8,  8,
												        4,  4,  4,  4,
												        0,  0,  0,  0);

            __m256i upper_alpha_mask = _mm256_set_epi8(28, 28, 28, 28,
            										   24, 24, 24, 24,
            										   20, 20, 20, 20,
            										   16, 16, 16, 16,
            										   0x80, 0x80, 0x80, 0x80,
            										   0x80, 0x80, 0x80, 0x80,
            										   0x80, 0x80, 0x80, 0x80,
            										   0x80, 0x80, 0x80, 0x80);

            __m256i lower_alpha_foreground = _mm256_shuffle_epi8(fg, lower_alpha_mask);
            __m256i upper_alpha_foreground = _mm256_shuffle_epi8(fg, upper_alpha_mask);

            __m256i inverse_lower_alpha_foreground = _mm256_sub_epi8(c256, lower_alpha_foreground);
            __m256i inverse_upper_alpha_foreground = _mm256_sub_epi8(c256, upper_alpha_foreground);

            __m256i lower_foreground_pixels = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(fg, 0));
            __m256i upper_foreground_pixels = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(fg, 1));

            __m256i lower_background_pixels = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(bg, 0));
            __m256i upper_background_pixels = _mm256_cvtepu8_epi16(_mm256_extracti128_si256(bg, 1));     

            /*lower_foreground_pixels = _mm256_mullo_epi16(lower_foreground_pixels, lower_alpha_foreground);
            upper_foreground_pixels = _mm256_mullo_epi16(upper_foreground_pixels, upper_alpha_foreground);

            lower_background_pixels = _mm256_mullo_epi16(lower_background_pixels, inverse_lower_alpha_foreground);
            upper_background_pixels = _mm256_mullo_epi16(upper_background_pixels, inverse_upper_alpha_foreground);

            lower_foreground_pixels = _mm256_add_epi16(lower_foreground_pixels, lower_background_pixels);
            upper_foreground_pixels = _mm256_add_epi16(upper_foreground_pixels, upper_background_pixels);*/

            /*__m256i lower_mask = _mm256_set_epi8( 0x80, 0x80, 0x80, 0x80,
            							          0x80, 0x80, 0x80, 0x80,
            							          0x80, 0x80, 0x80, 0x80,
            							          0x80, 0x80, 0x80, 0x80,
            							          15, 13, 11,  9,
            							           7,  5,  3,  1,
            							          0x80, 0x80, 0x80, 0x80,
            							          0x80, 0x80, 0x80, 0x80);

            __m256i upper_mask = _mm256_set_epi8( 31, 29, 27, 25,
            							          23, 21, 19, 17,
            							          0x80, 0x80, 0x80, 0x80,
            							          0x80, 0x80, 0x80, 0x80,
            							          0x80, 0x80, 0x80, 0x80,
            							          0x80, 0x80, 0x80, 0x80,
            							          0x80, 0x80, 0x80, 0x80,
            							          0x80, 0x80, 0x80, 0x80);            

            __m256i lower_lower_result = _mm256_shuffle_epi8(lower_foreground_pixels, lower_mask);
            __m256i lower_upper_result = _mm256_shuffle_epi8(lower_foreground_pixels, upper_mask);

			__m256i upper_lower_result = _mm256_shuffle_epi8(upper_foreground_pixels, lower_mask);
			__m256i upper_upper_result = _mm256_shuffle_epi8(upper_foreground_pixels, upper_mask);

			__m128 lower_result = _mm_movehl_ps((__m128)(_mm256_extracti128_si256(lower_upper_result, 1)), (__m128)(_mm256_extracti128_si256(lower_lower_result, 0)));
			__m128 upper_result = _mm_movehl_ps((__m128)(_mm256_extracti128_si256(upper_upper_result, 1)), (__m128)(_mm256_extracti128_si256(upper_lower_result, 0)));*/

			// _mm_storeu_si128((__m128i*)(background_picture->pixels + x_background * background_picture->wigth_screen + y_background),     (__m128i)lower_result);

			//_mm_storeu_si128((__m128i*)(background_picture->pixels + x_background * background_picture->wigth_screen + y_background),     (__m128i)background_pixels);      

			y_background += 4;
		}
		++x_background;
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

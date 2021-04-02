#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "TXlib.h"
#include <emmintrin.h>

enum Statuses_type {
	ALL_IS_OKEY    = 0,
	ESCAPE_PRESSED = 1,
	BAD_POINTERS   = 2,
};

struct Mandelbrot_params {
	int width_screen;
	int height_screen;
	int max_steps;
	int sqr_max_radius;
	float scale;
	float x_center, y_center;
	float small_delta, big_delta;
	float dx, dy;
};

struct Screen_type {
	RGBQUAD* screen_buffer;
	Mandelbrot_params* parameters;
};

struct Coordinates {
	__m128 x;
	__m128 y;
	__m128i step;
};

const int PACK = 4;

const RGBQUAD BLACK_COLOUR = RGBQUAD {0, 0, 0, 255};

Screen_type* screen_new(Mandelbrot_params* parameters);            

Statuses_type start_work_with_window(const Mandelbrot_params* parameters);

void screen_update(Screen_type* screen);

Statuses_type draw_mandelbrot(Screen_type* screen);

Statuses_type draw_points(Screen_type* screen);

bool is_escape_pressed();

bool is_zoom_in_pressed();

bool is_zoom_out_pressed();

bool is_shift_up_pressed();

bool is_shift_down_pressed();

bool is_shift_left_pressed();

bool is_shift_right_pressed();

bool is_shift_pressed();

Coordinates counting_new_coordinates(const Coordinates old_coordinates, const Mandelbrot_params* parameters);

Statuses_type set_pixel_color(Screen_type* screen, const int x_coord, const int y_coord, const RGBQUAD color);

RGBQUAD calculate_colour(const int number_of_step, const int max_count_of_steps, const unsigned char char_transparency);

Statuses_type screen_delete(Screen_type* screen);

#endif
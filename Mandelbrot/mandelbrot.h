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
	int wigth_screen;
	int height_screen;
	int max_steps;
	int sqr_max_radius;
	float scale;
	float x_coord_center, y_coord_center;
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

const RGBQUAD BLACK_COLOUR = RGBQUAD {0, 0, 0, 255};

const float ROI_X = -1.325f,
            ROI_Y = 0;    

//Statuses_type screen_new(Mandelbrot_params* parameters, Screen_type* screen);            
Screen_type* screen_new(Mandelbrot_params* parameters);            

Statuses_type start_work_with_window(const Mandelbrot_params* parameters);

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

bool is_1_pressed();

Coordinates counting_new_coordinates(const Coordinates old_coordinates, const Mandelbrot_params* parameters);

void get_colour(RGBQUAD point_colour[4], const __m128i steps, const Mandelbrot_params* parameters);

Statuses_type set_pixel_color(Screen_type* screen, const int x_coord, const int y_coord, const RGBQUAD color);

RGBQUAD calculate_colour(int number_of_step, const int max_count_of_steps, const unsigned char char_transparency);

#endif
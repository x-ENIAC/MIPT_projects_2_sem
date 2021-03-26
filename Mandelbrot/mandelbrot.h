#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "TXlib.h"
#include <emmintrin.h>

enum Statuses_type {
	ALL_IS_OKEY    = 0,
	ESCAPE_PRESSED = 10
};

struct Mandelbrot_params {
	int wigth_screen;
	int height_screen;
	int max_steps;
	int sqr_max_radius;
	float scale;
	float dx, dy;
};

struct Coordinates {
	__m128 x;
	__m128 y;
	__m128i step;
};




const float xC = 0.f, yC = 0.f;

const float ROI_X = -1.325f,
            ROI_Y = 0;    

Statuses_type start_work_with_window(Mandelbrot_params* parameters);

Statuses_type draw_mandelbrot(Mandelbrot_params* parameters);

Statuses_type draw_points(Mandelbrot_params* parameters);

bool is_escape_pressed();

Coordinates counting_new_coordinates(Coordinates old_coordinates, Mandelbrot_params* parameters);

void get_colour(RGBQUAD point_colour[4], __m128i steps, Mandelbrot_params* parameters);

#endif
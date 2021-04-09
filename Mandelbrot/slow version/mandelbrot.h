#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "TXlib.h"

enum Statuses_type {
	ALL_IS_OKEY    = 0,
	ESCAPE_PRESSED = 10
};

struct Mandelbrot_params {
	int width_screen;
	int height_screen;
	int max_steps;
	float sqr_max_radius;
	float scale;
	float x_center, y_center;
	float small_delta, big_delta;
	float dx, dy;
};


struct Coordinates {
	float x;
	float y;
	int step;
};


const float xC = 0.f, yC = 0.f;

const float ROI_X = -1.325f,
            ROI_Y = 0;    

Statuses_type start_work_with_window(Mandelbrot_params* parameters);

Statuses_type draw_mandelbrot(Mandelbrot_params* parameters);

Statuses_type draw_points(Mandelbrot_params* parameters);

bool is_escape_pressed();

bool is_zoom_in_pressed();

bool is_zoom_out_pressed();

bool is_shift_up_pressed();

bool is_shift_down_pressed();

bool is_shift_left_pressed();

bool is_shift_right_pressed();

bool is_shift_pressed();

Coordinates counting_new_coordinates(Coordinates old_coordinates, Mandelbrot_params* parameters);

RGBQUAD get_colour(Coordinates* coordinates, Mandelbrot_params* parameters);

#endif
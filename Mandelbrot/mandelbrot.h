#ifndef MANDELBROT_H
#define MANDELBROT_H

#include "TXlib.h"

enum Statuses_type {
	ALL_IS_OKEY    = 0,
	ESCAPE_PRESSED = 10
};

struct Mandelbrot_params {
	int wigth_screen;   //= 800;
	int height_screen; // = 400;
	int max_steps;
	int sqr_max_radius;
};

struct Coordinates {
	float x;
	float y;
	int step;
};


    const int   nMax  = 256;
    const float dx    = 1/800.f, dy = 1/800.f;
    const float r2Max = 100.f;

    const float xC = 0.f, yC = 0.f, scale = 1.f;

const float ROI_X = -1.325f,
            ROI_Y = 0;    

Statuses_type start_work_with_window(Mandelbrot_params* parameters);

Statuses_type draw_mandelbrot(Mandelbrot_params* parameters);

Statuses_type draw_points(Mandelbrot_params* parameters);

bool is_escape_pressed();

Coordinates counting_new_coordinates(Coordinates old_coordinates, Mandelbrot_params* parameters);

RGBQUAD get_colour(Coordinates* coordinates, Mandelbrot_params* parameters);

#endif
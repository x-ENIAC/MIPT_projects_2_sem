
#include "TXlib.h"
#include "mandelbrot.h"
#include <math.h>

Statuses_type start_work_with_window(Mandelbrot_params* parameters) {
	txCreateWindow(parameters->wigth_screen, parameters->height_screen);
	txBegin();		// Blocks window image refresh to avoid flickering ©Ded32

	return ALL_IS_OKEY;
}

Statuses_type draw_mandelbrot(Mandelbrot_params* parameters) {
    bool is_window_open = true;
    	while(is_window_open) {
			if(is_escape_pressed()) {
				is_window_open = false;
				break;
			} //else if()

			Statuses_type now_status = draw_points(parameters);
			if(now_status == ESCAPE_PRESSED) {
				is_window_open = false;
				break;			
			}

			printf("\t\r%.0lf", txGetFPS());
	        txSleep();
		}

	txSleep();

    return ALL_IS_OKEY;
}

Statuses_type draw_points(Mandelbrot_params* parameters) {
	typedef RGBQUAD (&Screen_buffer_type) [parameters->height_screen][parameters->wigth_screen];
	Screen_buffer_type screen_buffer = (Screen_buffer_type) *txVideoMemory();	

	for(int y = 0; y < parameters->height_screen; ++y) {
		if(is_escape_pressed())
			return ESCAPE_PRESSED;

		float start_x = ( (			- parameters->wigth_screen  / 2.0) * parameters->dx + ROI_X + xC ) * parameters->scale, 
			  start_y = ( ((float)y - parameters->height_screen / 2.0) * parameters->dy + ROI_Y + yC ) * parameters->scale;

		for(int x = 0; x < parameters->wigth_screen; ++x, start_x += parameters->dx) {
			if(is_escape_pressed())
				return ESCAPE_PRESSED;

			Coordinates new_coordinates = counting_new_coordinates(Coordinates{start_x, start_y, 0}, parameters);
			RGBQUAD point_colour = get_colour(&new_coordinates, parameters);
			screen_buffer[y][x] = point_colour;
		}
	}

	return ALL_IS_OKEY;
}

bool is_escape_pressed() {
	return GetAsyncKeyState(VK_ESCAPE);
}

Coordinates counting_new_coordinates(Coordinates old_coordinates, Mandelbrot_params* parameters) {
	Coordinates new_coordinates = old_coordinates;

	for(int step = 0; step < parameters->max_steps; ++step) {
		float mult_xy = new_coordinates.x * new_coordinates.y;
		float sqr_x   = new_coordinates.x * new_coordinates.x;
		float sqr_y   = new_coordinates.y * new_coordinates.y;

		float sqr_distance = sqr_x + sqr_y;

		if(sqr_distance > parameters->sqr_max_radius) {
			new_coordinates.step = step;
			break;
		}

		new_coordinates.x =  sqr_x  -  sqr_y  + old_coordinates.x;
		new_coordinates.y = mult_xy + mult_xy + old_coordinates.y;
	}

	return new_coordinates;
}

RGBQUAD get_colour(Coordinates* coordinates, Mandelbrot_params* parameters) {
	float transparency = sqrtf(sqrtf((float)coordinates->step / (float)parameters->max_steps)) * 255.f;
	unsigned char char_transparency = (unsigned char)transparency;

	if(coordinates->step < parameters->max_steps)
		return RGBQUAD {(unsigned char)(255 - char_transparency), (unsigned char)(char_transparency % 2 * 64), char_transparency};

	return RGBQUAD {0, 0, 0, 255};
}
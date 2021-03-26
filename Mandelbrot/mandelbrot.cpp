
#include "TXlib.h"
#include "mandelbrot.h"
#include <math.h>
#include <emmintrin.h>

const __m128 _3210_dx  = _mm_set_ps  (3.f, 2.f, 1.f, 0.f);
const __m128 _255  	   = _mm_set_ps1 (255.f);

Statuses_type start_work_with_window(Mandelbrot_params* parameters) {
	txCreateWindow(parameters->wigth_screen, parameters->height_screen);
	Win32::_fpreset();
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

		double start_x = ( (	      - parameters->wigth_screen  / 2.0) * parameters->dx + ROI_X + xC ) * parameters->scale, 
			   start_y = ( ((double)y - parameters->height_screen / 2.0) * parameters->dy + ROI_Y + yC ) * parameters->scale;

		for(int x = 0; x < parameters->wigth_screen; x += 4, start_x += parameters->dx * 4) {
			if(is_escape_pressed())
				return ESCAPE_PRESSED;

			__m128 dx_for_mult = _mm_set_ps1(parameters->dx);
				   dx_for_mult = _mm_mul_ps (_3210_dx, dx_for_mult);

			Coordinates now_coordinates = {};

			now_coordinates.y = 		    _mm_set_ps1(start_y);
			now_coordinates.x = _mm_add_ps( _mm_set_ps1(start_x), dx_for_mult);

			Coordinates new_coordinates = counting_new_coordinates(now_coordinates, parameters);			

			RGBQUAD point_colour[4] = {};
			get_colour(point_colour, new_coordinates.step, parameters);

			for(int ind = 0; ind < 4; ++ind)
				screen_buffer[y][x + ind] = point_colour[ind];
			
			
		}
	}

	return ALL_IS_OKEY;
}

bool is_escape_pressed() {
	return GetAsyncKeyState(VK_ESCAPE);
}

Coordinates counting_new_coordinates(Coordinates old_coordinates, Mandelbrot_params* parameters) {
	Coordinates new_coordinates = old_coordinates;
	new_coordinates.step = _mm_setzero_si128();

	__m128 max_distance = _mm_set_ps1((double)parameters->sqr_max_radius);

	for(int step = 0; step < parameters->max_steps; ++step) {
		__m128 mult_xy = _mm_mul_ps(new_coordinates.x, new_coordinates.y);
		__m128 sqr_x   = _mm_mul_ps(new_coordinates.x, new_coordinates.x);
		__m128 sqr_y   = _mm_mul_ps(new_coordinates.y, new_coordinates.y);

		__m128 sqr_distance = _mm_add_ps(sqr_x, sqr_y);	

		__m128 cmp_steps = _mm_cmple_ps(sqr_distance, max_distance);
		int mask_steps = _mm_movemask_ps(cmp_steps);
		if(!mask_steps)
			break;

		new_coordinates.step = _mm_sub_epi32(new_coordinates.step, _mm_castps_si128 (cmp_steps));

		new_coordinates.x = _mm_add_ps(_mm_sub_ps(sqr_x,     sqr_y), old_coordinates.x);
		new_coordinates.y = _mm_add_ps(_mm_add_ps(mult_xy, mult_xy), old_coordinates.y);
	}

	return new_coordinates;
}

void get_colour(RGBQUAD point_colour[4], __m128i steps, Mandelbrot_params* parameters) {
	__m128 many_max_steps = _mm_set_ps1((float)parameters->max_steps);
	__m128 transparency   = _mm_mul_ps(_mm_sqrt_ps(_mm_sqrt_ps(_mm_div_ps(_mm_cvtepi32_ps(steps), many_max_steps))), _255);

	for(int ind = 0; ind < 4; ++ind) {
		int*   pointer_steps 		= (int*)   &steps;
        float* pointer_transparency = (float*) &transparency;
        
        unsigned char char_transparency = (unsigned char)pointer_transparency[ind];

		if(pointer_steps[ind] < parameters->max_steps)
			point_colour[ind] = RGBQUAD {(unsigned char)(255 - char_transparency), (unsigned char)(char_transparency % 2 * 64), char_transparency};
		else
			point_colour[ind] = RGBQUAD {0, 0, 0, 255};
	} 

	return;
}
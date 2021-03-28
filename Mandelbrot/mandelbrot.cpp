

#include "TXlib.h"
#include "mandelbrot.h"
#include <math.h>
#include <emmintrin.h>

#define CHECK_STATUS									\
	if(status != ALL_IS_OKEY) {							\
		printf("Bad; status = %d\n", status);			\
	}

const __m128 _3210_dx  = _mm_set_ps  (3.f, 2.f, 1.f, 0.f);
const __m128 _255  	   = _mm_set_ps1 (255.f);
const __m128 _160  	   = _mm_set_ps1 (160.f);

Screen_type* screen_new(Mandelbrot_params* parameters) {
	if(!parameters)
		return NULL;
	
	Screen_type* screen = (Screen_type*)calloc(1, sizeof(Screen_type));
	screen->parameters = parameters;
	screen->screen_buffer = (RGBQUAD*)txVideoMemory();

	return screen;
}

Statuses_type start_work_with_window(const Mandelbrot_params* parameters) {
	txCreateWindow(parameters->wigth_screen, parameters->height_screen);
	Win32::_fpreset();
	txBegin();		// Blocks window image refresh to avoid flickering ©Ded32

	return ALL_IS_OKEY;
}

Statuses_type draw_mandelbrot(Screen_type* screen) {
    bool is_window_open = true;
    	while(is_window_open) {
			if(is_escape_pressed()) {
				is_window_open = false;
				break;
			} else if(is_zoom_in_pressed()) {
				screen->parameters->scale 		   += screen->parameters->dx * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
			} else if(is_zoom_out_pressed()) {
				screen->parameters->scale 		   -= screen->parameters->dx * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
			} else if(is_shift_right_pressed()) {
				screen->parameters->x_coord_center += screen->parameters->dx * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
			} else if(is_shift_left_pressed()) {
				screen->parameters->x_coord_center -= screen->parameters->dx * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
			} else if(is_shift_down_pressed()) {
        		screen->parameters->y_coord_center += screen->parameters->dy * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
        	} else if(is_shift_up_pressed()) {
        		screen->parameters->y_coord_center -= screen->parameters->dy * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
        	} else if(is_1_pressed()) {				// First optimization 
				printf("First!\n");
				screen->parameters->sqr_max_radius = 4;
			}

			Statuses_type now_status = draw_points(screen);
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

Statuses_type draw_points(Screen_type* screen) {
	for(int y = 0; y < screen->parameters->height_screen; ++y) {
		if(is_escape_pressed())
			return ESCAPE_PRESSED;

		float start_x = ( (	         - screen->parameters->wigth_screen  / 2.0) * screen->parameters->dx + ROI_X + screen->parameters->x_coord_center ) * screen->parameters->scale, 
			   start_y = ( ((float)y - screen->parameters->height_screen / 2.0) * screen->parameters->dy + ROI_Y + screen->parameters->y_coord_center ) * screen->parameters->scale;

		for(int x = 0; x < screen->parameters->wigth_screen; x += 4, start_x += screen->parameters->dx * 4) {
			if(is_escape_pressed())
				return ESCAPE_PRESSED;

			__m128 dx_for_mult = _mm_set_ps1(screen->parameters->dx);
				   dx_for_mult = _mm_mul_ps (_3210_dx, dx_for_mult);

			Coordinates now_coordinates = {};

			now_coordinates.y = 		    _mm_set_ps1(start_y);
			now_coordinates.x = _mm_add_ps( _mm_set_ps1(start_x), dx_for_mult);

			Coordinates new_coordinates = counting_new_coordinates(now_coordinates, screen->parameters);			

			RGBQUAD point_colour[4] = {};
			get_colour(point_colour, new_coordinates.step, screen->parameters);

			Statuses_type status = ALL_IS_OKEY;
			for(int ind = 0; ind < 4; ++ind) {
				//printf("%d\n", y * parameters->wigth_screen + x + ind);
				//screen->screen_buffer[y * screen->parameters->wigth_screen + x + ind] = point_colour[ind];
				status = set_pixel_color(screen, x + ind, y, point_colour[ind]);
				CHECK_STATUS
			}			
		}
	}

	return ALL_IS_OKEY;
}

bool is_escape_pressed() {
	return GetAsyncKeyState(VK_ESCAPE);
}

bool is_zoom_in_pressed() {
	return txGetAsyncKeyState('A');
}

bool is_zoom_out_pressed() {
	return txGetAsyncKeyState('Z');
}

bool is_shift_up_pressed() {
	return txGetAsyncKeyState (VK_UP);
}

bool is_shift_down_pressed() {
	return txGetAsyncKeyState (VK_DOWN);
}

bool is_shift_left_pressed() {
	return txGetAsyncKeyState (VK_LEFT);
}

bool is_shift_right_pressed() {
	return txGetAsyncKeyState (VK_RIGHT);
}

bool is_shift_pressed() {
	return txGetAsyncKeyState (VK_SHIFT);
}

bool is_1_pressed() {
	return txGetAsyncKeyState ('1');
}


Coordinates counting_new_coordinates(const Coordinates old_coordinates, const Mandelbrot_params* parameters) {
	Coordinates new_coordinates = old_coordinates;
	new_coordinates.step = _mm_setzero_si128();

	__m128 max_distance = _mm_set_ps1( (float)parameters->sqr_max_radius);

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

void get_colour(RGBQUAD point_colour[4], const __m128i steps, const Mandelbrot_params* parameters) {
	__m128 many_max_steps = _mm_set_ps1((float)parameters->max_steps);
	__m128 transparency = _mm_mul_ps(_mm_sqrt_ps(_mm_sqrt_ps(_mm_div_ps(_mm_cvtepi32_ps(steps), many_max_steps))), _160);

	for(int ind = 0; ind < 4; ++ind) {
		int*   pointer_steps 		= (int*)   &steps;
        float* pointer_transparency = (float*) &transparency;
        
        unsigned char char_transparency = (unsigned char)pointer_transparency[ind];

		if(pointer_steps[ind] < parameters->max_steps)
			point_colour[ind] = RGBQUAD {(unsigned char)((255 - char_transparency) * 2), (unsigned char)(char_transparency % 21 * 64), (unsigned char)(char_transparency % 200)};
		else
			point_colour[ind] = BLACK_COLOUR;
	} 

	return;
}

Statuses_type set_pixel_color(Screen_type* screen, const int x_coord, const int y_coord, const RGBQUAD color) {
	if(!screen)
		return BAD_POINTERS;

	screen->screen_buffer[y_coord * screen->parameters->wigth_screen + x_coord] = color;
	return ALL_IS_OKEY;
}
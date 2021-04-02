

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

	screen_update(screen);

	return screen;
}

Statuses_type start_work_with_window(const Mandelbrot_params* parameters) {
	txCreateWindow(parameters->width_screen, parameters->height_screen);
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
			}

			screen_update(screen);

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

void screen_update(Screen_type* screen) {
	if(is_zoom_in_pressed()) {
		screen->parameters->scale    += screen->parameters->dx * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
	} else if(is_zoom_out_pressed()) {
		screen->parameters->scale 	 -= screen->parameters->dx * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
	} else if(is_shift_right_pressed()) {
		screen->parameters->x_center += screen->parameters->dx * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
	} else if(is_shift_left_pressed()) {
		screen->parameters->x_center -= screen->parameters->dx * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
	} else if(is_shift_down_pressed()) {
        screen->parameters->y_center += screen->parameters->dy * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
    } else if(is_shift_up_pressed()) {
        screen->parameters->y_center -= screen->parameters->dy * (is_shift_pressed() ? screen->parameters->big_delta : screen->parameters->small_delta);
    }

	screen->parameters->dx = (screen->parameters->scale / (float)screen->parameters->width_screen);
	screen->parameters->dy = (screen->parameters->scale / (float)screen->parameters->height_screen);
}

Statuses_type draw_points(Screen_type* screen) {
	for(int y = 0; y < screen->parameters->height_screen; ++y) {
		if(is_escape_pressed())
			return ESCAPE_PRESSED;

		float start_x = screen->parameters->x_center - (float)(screen->parameters->scale / 2.0);
		float start_y = screen->parameters->y_center - (float)(screen->parameters->scale / 2.0);

		for(int x = 0; x + 3 < screen->parameters->width_screen; x += PACK) {
			if(is_escape_pressed())
				return ESCAPE_PRESSED;

			__m128 dx_for_mult = _mm_set_ps1(screen->parameters->dx);

			Coordinates now_coordinates = {};

			now_coordinates.x = _mm_add_ps(_mm_set_ps1((float)x), _3210_dx);
			now_coordinates.x = _mm_add_ps(_mm_mul_ps(dx_for_mult, now_coordinates.x), _mm_set_ps1(start_x));
			now_coordinates.y = _mm_set_ps1(start_y + (float)y * screen->parameters->dy);

			Coordinates new_coordinates = counting_new_coordinates(now_coordinates, screen->parameters);			

			RGBQUAD point_colour[PACK] = {};

			__m128 many_max_steps = _mm_set_ps1((float)screen->parameters->max_steps);
			__m128 transparency = _mm_mul_ps(_mm_sqrt_ps(_mm_sqrt_ps(_mm_div_ps(_mm_cvtepi32_ps(new_coordinates.step), many_max_steps))), _160);

			const int* pointer_steps = (const int*) &new_coordinates.step;
    		const float* pointer_transparency = (const float*) &transparency;			

			for(int ind = 0; ind < PACK; ++ind)
				point_colour[ind] = calculate_colour(pointer_steps[ind], screen->parameters->max_steps, (unsigned char)pointer_transparency[ind]);			

			Statuses_type status = ALL_IS_OKEY;
			for(int ind = 0; ind < PACK; ++ind) {
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


Coordinates counting_new_coordinates(const Coordinates old_coordinates, const Mandelbrot_params* parameters) {
	Coordinates new_coordinates = old_coordinates;
	new_coordinates.step = _mm_setzero_si128();

	__m128 max_distance = _mm_set_ps1((float)parameters->sqr_max_radius);

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

Statuses_type set_pixel_color(Screen_type* screen, const int x_coord, const int y_coord, const RGBQUAD color) {
	if(!screen)
		return BAD_POINTERS;

	screen->screen_buffer[y_coord * screen->parameters->width_screen + x_coord] = color;
	return ALL_IS_OKEY;
}

RGBQUAD calculate_colour(const int number_of_step, const int max_count_of_steps, const unsigned char char_transparency) {
	if(number_of_step < max_count_of_steps)
		return RGBQUAD {(unsigned char)((255 - char_transparency) * 1.8), (unsigned char)(char_transparency % 21 * 64), (unsigned char)(char_transparency % 200)};

	return BLACK_COLOUR;
}

Statuses_type screen_delete(Screen_type* screen) {
	if(!screen)
		return ALL_IS_OKEY;

	free(screen);

	return ALL_IS_OKEY;
}
/* лдвш, 2 школа */
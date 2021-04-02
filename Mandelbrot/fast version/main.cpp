#include "TXLib.h"
#include <stdio.h>
#include "mandelbrot.h"

#define CHECK_STATUS									\
	if(status != ALL_IS_OKEY) {							\
		printf("Bad; status = %d\n", status);			\
	}

int main() {
 	Mandelbrot_params parameters = {
		400,			// wigth_screen
		400,			// height_screen
		10000,			// max_steps
		100,			// sqr_max_radius
		5.5,			// scale
		0.f,			// x_coord_center
		0.f,			// y_coord_center
		5.f,			// small_delta
		50.f,			// big_delta
		1 / 800.f,
		1 / 800.f
	};

	Statuses_type status = ALL_IS_OKEY;
	status = start_work_with_window(&parameters);
    CHECK_STATUS

	Screen_type* screen = screen_new(&parameters);    

	draw_mandelbrot(screen);

    screen_delete(screen);
    CHECK_STATUS

	return ALL_IS_OKEY;
}
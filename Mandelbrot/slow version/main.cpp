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
		100.0,			// sqr_max_radius
		5.5,			// scale
		0.f,			// x_coord_center
		0.f,			// y_coord_center
		5.f,			// small_delta
		50.f,			// big_delta
		1 / 800.f,
		1 / 800.f
	};

	Statuses_type status = start_work_with_window(&parameters);
	CHECK_STATUS

	status = draw_mandelbrot(&parameters);
	CHECK_STATUS


	return ALL_IS_OKEY;
}
#include "TXLib.h"
#include <stdio.h>
#include "mandelbrot.h"

#define CHECK_STATUS									\
	if(status != ALL_IS_OKEY) {							\
		printf("Bad; status = %d\n", status);			\
	}

int main() {
 	Mandelbrot_params parameters = {
		800,			// wigth_screen
		600,			// height_screen
		2000,			// max_steps
		100,			// sqr_max_radius
		1.0,			// scale
		1 / 800.f,		// dx
		1 / 800.f		// dy
	};

    Statuses_type status = start_work_with_window(&parameters);
    CHECK_STATUS

    status = draw_mandelbrot(&parameters);
    CHECK_STATUS


	return ALL_IS_OKEY;
}
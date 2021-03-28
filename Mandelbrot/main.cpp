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
		//1 / 800.f,		// dx
		//1 / 800.f,		// dy
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

    status = draw_mandelbrot(screen);
    CHECK_STATUS

    //status


	return ALL_IS_OKEY;
}
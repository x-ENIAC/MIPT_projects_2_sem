#include "TXLib.h"
#include <stdio.h>
#include "mandelbrot.h"

#define CHECK_STATUS									\
	if(status != ALL_IS_OKEY) {							\
		printf("Bad; status = %d\n", status);			\
	}

int main() {
 	Mandelbrot_params parameters = {
		800,
		400,
		2000,
		100
	};

    Statuses_type status = start_work_with_window(&parameters);
    CHECK_STATUS

    status = draw_mandelbrot(&parameters);
    CHECK_STATUS


	return ALL_IS_OKEY;
}
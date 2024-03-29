/*
 * task2_waste_time.c
 *
 * this is the main file for all functions that are used by the threads
 *
 * Created on: Nov 26, 2023
 * Author: 	Valentin Lairich
 * Author:	Sami Hamza
 * Version:	1.0
 * Updated:	Nov 26, 2023
 */

#include <sys/neutrino.h>
#include <inttypes.h>
#include <sys/syspage.h>
#include <sys/procmgr.h>
#include <time.h>
#include <task3_waste_time.h>
#include <main.h>

//
int calibrated = 0;

void waste_time(unsigned int msecs) {
	// create the timespec struct and write time
	struct timespec when;
	nsec2timespec(&when,msecs*1000000);
	// waste time by calling nanospin (performs calibrated do... while...)
	CHECK_SUCCESS(nanospin(&when));
	return;
}


void waste_time_calibrate(void) {
	if (!calibrated) {
		// Enable IO-priviliges
		CHECK_SUCCESS(procmgr_ability(0,
				PROCMGR_ADN_NONROOT|PROCMGR_AOP_ALLOW|PROCMGR_AID_IO,
				PROCMGR_AID_EOL));
		CHECK_SUCCESS(ThreadCtl( _NTO_TCTL_IO, 0 ));
		// Calibrate with interrupts disabled
		CHECK_SUCCESS(nanospin_calibrate(1));
		calibrated = 1;
	}
	return;
}

int task3_main() {
	// calibration of the waste time
	waste_time_calibrate();

	// array of demanded times to waste in ms
	int waste_times[7] = {1, 2, 5, 10, 20};

	for (int i = 0; i < 5; i++)  {
		printf("waste %d ms\n", waste_times[i]);
		delay(10);
		waste_time(waste_times[i]);
		delay(10);
	}

	return EXIT_SUCCESS;
}

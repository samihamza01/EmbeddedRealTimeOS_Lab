/*
* Praktikum Embedded- und Echtzeitbetriebssysteme 23/24
* Gruppe: xy
*
* This is the main thread for testing everything
*
* Created on: Nov 25, 2023
* Author: 	Valentin Lairich
* Author:	Sami Hamza
* Version:	1.0
* Updated:	Nov 25, 2023
*/
#include <sim_main.h>
#include <thread_functions.h>
#include <task6_stack_tracing.h>

int main(void) {
	sleep(1);
	printf("Starting simulation!!!\n");

	task6_main();

	return EXIT_SUCCESS;
}

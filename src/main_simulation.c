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

#include <main.h>
#include <task2_threads_and_cond_vars.h>
#include <task3_waste_time.h>
#include <task4_cooperating_tasks.h>
#include <task6_stack_tracing.h>

int main(void) {
	sleep(2);
	printf("Starting main!\n");

	//task2_main(); // Uncomment for task 2
	//task3_main(); // Uncomment for task 3
	//task4_main(); // Uncomment for task 4
	task6_main(); // Uncomment for task 6

	return EXIT_SUCCESS;
}

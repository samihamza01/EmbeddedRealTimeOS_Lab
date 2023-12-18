/*
* Praktikum Embedded- und Echtzeitbetriebssysteme 23/24
* Gruppe: xy
*
* This is the main thread for testing everything
*
* Created on: Nov 25, 2023
* Author: 	Valentin Lairich
* Author:	-
* Version:	1.0
* Updated:	Nov 25, 2023
*/
#include <sim_main.h>
#include <thread_functions.h>

int main(void) {
	Room kitchen;

	puts("Starting simulation !!!"); /* prints Hello World!!! */

	// Initialize the room
	//CHECK_SUCCESS(init_room(&kitchen, 0));
	init_room(&kitchen, 0);


	// Initialize four persons in the kitchen
	Person persons[4];
	pthread_t tid[4];

	for(int i = 0; i < 4; i++) {
		//CHECK_SUCCESS(init_person(&persons[i], i, &kitchen, 0));
		init_person(&persons[i], i, &kitchen, 0);

		// Create a thread for each person
		//CHECK_SUCCESS(pthread_create(&tid[i], NULL, person_thread, &persons[i]));
		pthread_create(&tid[i], NULL, person_thread, &persons[i]);
	}

	// create a monitor thread
	pthread_t kitchen_monitor;
	//CHECK_SUCCESS(pthread_create(&kitchen_monitor, NULL, room_monitor_thread, &kitchen));
	pthread_create(&kitchen_monitor, NULL, room_monitor_thread, &kitchen);
	//CHECK_SUCCESS(pthread_join(kitchen_monitor, NULL));
	pthread_join(kitchen_monitor, NULL);

	for(int i = 0; i < 4; i++) {
		//CHECK_SUCCESS(pthread_join(tid[i], NULL));
		pthread_join(tid[i], NULL);
	}

	return EXIT_SUCCESS;
}

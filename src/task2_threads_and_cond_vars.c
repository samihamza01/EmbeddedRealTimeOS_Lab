/*
 * task2_threads_and_cond_vars.c
 *
 * Task 2: Sourcefile for functionalities from task 2.
 *
 * Created on: Nov 26, 2023
 * Author: 	Valentin Lairich
 * Author:	Sami Hamza
 * Version:	1.0
 * Updated:	Nov 26, 2023
 */

#include <task2_threads_and_cond_vars.h>
#include <main.h>

int init_room(Room *room, int id){

	// Initialize mutex and condition variable
	pthread_mutex_init(&room->lock, NULL);
	pthread_cond_init(&room->room_notifier, NULL);

	// Initialize other members
	room->id = id;
	room->person_cnt = 0;

	return 0;
}

int destroy_room(Room *room){

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&room->lock);
    pthread_cond_destroy(&room->room_notifier);

	return 0;
}

int init_person(Person *person, int id, Room *room, int is_in_room){

	// Initialize mutex and condition variable
	pthread_mutex_init(&person->lock, NULL);

	// Initialize other members
	person->id = id;
	person->room = room;
	person->is_in_room = is_in_room;
	person->work_func = NULL;

	return 0;
}

int destroy_person(Person *person){

    // Destroy mutex and condition variable
    pthread_mutex_destroy(&person->lock);

	return 0;
}

// Thread Function Definitions ///////////////////////////////
void *person_thread(void* arg){
	Person *person = (Person *)arg;

	while(1) {
		// Zufällige Zeit für An- bzw. Abwesenheit generieren (1-30 Sekunden)
		int sleepTime = rand() % 30 + 1;
		sleep(sleepTime);

		pthread_mutex_lock(&person->lock);

		// Person betritt oder verlässt den Raum
		person->is_in_room = !person->is_in_room;

		pthread_mutex_unlock(&person->lock);

		pthread_mutex_lock(&person->room->lock);

		// Raum counter hoch oder runterzählen
		if(person->is_in_room == 0)
			person->room->person_cnt--;
		else
			person->room->person_cnt++;

		pthread_cond_signal(&person->room->room_notifier);
		pthread_mutex_unlock(&person->room->lock);

	}

	return NULL;
}
void *room_monitor_thread(void* arg){
	Room *room = (Room *)arg;

	while(1) {
		pthread_mutex_lock(&room->lock);
		pthread_cond_wait(&room->room_notifier,&room->lock);

		//puts("%i people in the room: %i!!!", room->person_cnt, room->id);
		printf("%d people in the room: %d!!!\n", room->person_cnt, room->id);


		pthread_mutex_unlock(&room->lock);
	}

	return NULL;
}

int task2_main(void) {
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

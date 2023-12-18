/*
 * sim_main.c
 *
 * main file for the "Embedded- und Echtzeitbetriebssysteme"
 * Praktikum
 *
 * Created on: Nov 25, 2023
 * Author: 	Valentin Lairich
 * Author:	-
 * Version:	1.0
 * Updated:	Nov 25, 2023
 */

#include <sim_main.h>


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

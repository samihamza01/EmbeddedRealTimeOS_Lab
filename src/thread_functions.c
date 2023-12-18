/*
 * thread_functions.c
 *
 * this is the main file for all functions that are used by the threads
 *
 * Created on: Nov 25, 2023
 * Author: 	Valentin Lairich
 * Author:	Sami Hamza
 * Version:	1.0
 * Updated:	Nov 25, 2023
 */

#include <thread_functions.h>

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

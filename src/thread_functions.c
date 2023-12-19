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
#include <task3_waste_time.h>
#include <time.h>
#include <semaphore.h>

// Defines ////////////////////////////////////////////
#define N 3

// Function Definitions ///////////////////////////////
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

void* clock_generator_thread(void* arg) {
	sem_t* clock_sem = (sem_t*)arg;
	// create the timespec struct
	struct timespec abs_time;
	// get the current time
	clock_gettime(CLOCK_REALTIME,&abs_time);
	// add the time step 2 ms
	abs_time.tv_sec += 0.002;
	abs_time.tv_nsec += 2000000;

	while(1) {
		CHECK_SUCCESS(clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&abs_time,NULL));
		sem_post(clock_sem);
	}
}

void* clock_consumer_thread(void* arg) {
	sem_t* waste_cpu_time_sems = (sem_t*)arg;
	int counter = 0;
	while(1) {
		sem_wait(waste_cpu_time_sems);
		waste_time(1);
		counter++;
		if (counter == N) {
			counter = 0;
			// increment semaphores for waste time threads
			sem_post(&waste_cpu_time_sems[0]);
			sem_post(&waste_cpu_time_sems[1]);
		}
	}
}

void* waste_cpu_time_1_thread(void* arg) {
	sem_t* waste_cpu_time_sem = (sem_t*)arg;
	while(1) {
		sem_wait(waste_cpu_time_sem);
		waste_time(4);
	}
}

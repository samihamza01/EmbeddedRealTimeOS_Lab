/*
 * task2_threads_and_cond_vars.h
 *
 * Task 2: Puplic interface for functionalities from task 2.
 *
 * Created on: Nov 26, 2023
 * Author: 	Valentin Lairich
 * Author:	Sami Hamza
 * Version:	1.0
 * Updated:	Nov 26, 2023
 */

#ifndef TASK2_THREADS_AND_COND_VARS_H_
#define TASK2_THREADS_AND_COND_VARS_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> /* pthread */

// STRUCTURES ///////////////////////////////////////////////////////////////////

// Struct to represent a room
typedef struct {
	pthread_mutex_t lock;
	pthread_cond_t	room_notifier;
	int id;
	int person_cnt;
}Room;

// Struct to represent a person
typedef struct {
	pthread_mutex_t lock;
	int id;
	Room *room;
	int is_in_room;
	int (*work_func)(void *);
}Person;


// FUNCTIONS /////////////////////////////////////////////////////////////////////

/* thread functions */
void* person_thread(void* arg);
void* room_monitor_thread(void* arg);
/* room functions */
int init_room(Room *room, int id);
int destroy_room(Room *room);

/* person functions */
int init_person(Person *person, int id, Room *room, int is_in_room);
int destroy_person(Person *person);

// @brief Function wrapper to perform specified tasks of task 2. SHould be called in main to perform the desired tasks.
// @returns Status exit code.
int task2_main();

#endif

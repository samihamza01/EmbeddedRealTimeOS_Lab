/*
 * sim_main.h
 *
 * main header file for the "Embedded- und Echtzeitbetriebssysteme"
 * Praktikum
 *
 * Created on: Nov 25, 2023
 * Author: 	Valentin Lairich
 * Co-Author:	-
 * Version:	1.0
 * Updated:	Nov 25, 2023
 */

#ifndef SIM_MAIN_H_
#define SIM_MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> /* pthread */
#include <unistd.h>
#include <string.h> /* strerror */
#include <errno.h>  /* errno */

// DEFINITIONS ///////////////////////////////////////////////////////////////////

#define CHECK_SUCCESS(call) \
do { int ret = call;\
	if ( ret != 0 ){\
		fprintf(stderr, "%s:%d %s failed: %s\n", __FILE__, __LINE__, #call, strerror(ret));\
		exit(1);\
	}} while(0);

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

// GLOBALS ///////////////////////////////////////////////////////////////////////
/*
//pthread_mutex_t room_lock = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t	room = PTHREAD_COND_INITIALIZER;
pthread_mutex_t condvar_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;
*/
// FUNCTIONS /////////////////////////////////////////////////////////////////////

/* room functions */
int init_room(Room *room, int id);
int destroy_room(Room *room);

/* person functions */
int init_person(Person *person, int id, Room *room, int is_in_room);
int destroy_person(Person *person);


#endif /* SIM_MAIN_H_ */

/*
 * task4_cooperating_tasks.c
 *
 * Task 4: Puplic interface for functionalities from task 4.
 *
 * Created on: Nov 26, 2023
 * Author: 	Valentin Lairich
 * Author:	Sami Hamza
 * Version:	1.0
 * Updated:	Nov 26, 2023
 */

#ifndef TASK4_COOPERATING_TASKS_H_
#define TASK4_COOPERATING_TASKS_H_

#include <semaphore.h>
#include <pthread.h> /* pthread */


void* clock_generator_thread(void* arg);
void* clock_consumer_thread(void* arg);
void* waste_cpu_time_thread(void* arg);

typedef struct {
	sem_t* clock_sem;
	sem_t* waste_cpu_time_sems;
}clock_consumer_args_t;

// @brief Function wrapper to perform specified tasks of task 4. SHould be called in main to perform the desired tasks.
// @returns Status exit code.
int task4_main();

#endif

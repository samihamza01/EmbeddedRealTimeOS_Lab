/*
 * task4_cooperating_tasks.c
 *
 * Task 4: Sourcefile for functionalities from task 4.
 *
 * Created on: Nov 26, 2023
 * Author: 	Valentin Lairich
 * Author:	Sami Hamza
 * Version:	1.0
 * Updated:	Nov 26, 2023
 */

#include <task4_cooperating_tasks.h>
#include <task3_waste_time.h>
#include <time.h>
#include <main.h>

// Defines ////////////////////////////////////////////
#define N 10

// Function Definitions ///////////////////////////////
void* clock_generator_thread(void* arg) {
	sem_t* clock_sem = (sem_t*)arg;
	// create the timespec struct
	struct timespec abs_time;
	// get the current time
	CHECK_SUCCESS(clock_gettime(CLOCK_REALTIME,&abs_time));
	while(1) {
		// add the time step 2 ms
		nsec2timespec(&abs_time,timespec2nsec(&abs_time) + 2000000);
		//abs_time.tv_sec += 0.002;
		//abs_time.tv_nsec += 2000000;
		CHECK_SUCCESS(clock_nanosleep(CLOCK_REALTIME,TIMER_ABSTIME,&abs_time,NULL));
		CHECK_SUCCESS(sem_post(clock_sem));
	}
}

void* clock_consumer_thread(void* arg) {
	clock_consumer_args_t* clock_consumer_args = (clock_consumer_args_t*) arg;
	sem_t* clock_sem = clock_consumer_args->clock_sem;
	sem_t* waste_cpu_time_sems = clock_consumer_args->waste_cpu_time_sems;
	int counter = 0;
	while(1) {
		CHECK_SUCCESS(sem_wait(clock_sem));
		waste_time(1);
		counter++;
		if (counter == N) {
			counter = 0;
			// increment semaphores for waste time threads
			CHECK_SUCCESS(sem_post(&waste_cpu_time_sems[0]));
			CHECK_SUCCESS(sem_post(&waste_cpu_time_sems[1]));
		}
	}
}

void* waste_cpu_time_thread(void* arg) {
	sem_t* waste_cpu_time_sem = (sem_t*)arg;
	while(1) {
		CHECK_SUCCESS(sem_wait(waste_cpu_time_sem));
		waste_time(4);
	}
}

int task4_main() {
	// calibrate waste time function
	waste_time_calibrate();

	// Create all threads: clock generator-, clock consumer- and two worker threads
	pthread_t clock_generator;
	pthread_t clock_consumer;
	pthread_t cpu_wast_time[2];

	// set scheduling params
	struct sched_param clock_generator_param;
	struct sched_param clock_consumer_param;
	struct sched_param cpu_wast_time_param[2];

	// get main thread priority
	struct sched_param main_thread_param;
	CHECK_SUCCESS(pthread_getschedparam(pthread_self(),NULL,&main_thread_param));
	int main_prio = main_thread_param.sched_curpriority;
	printf("Main thread priority: %d",main_prio);
	// Set priorities accordingly
	clock_generator_param.sched_priority = main_prio+2;
	clock_consumer_param.sched_priority = main_prio+1;
	cpu_wast_time_param[0].sched_priority = main_prio;
	cpu_wast_time_param[1].sched_priority = main_prio;



	// Define thread attributes
	pthread_attr_t clock_generator_attr;
	pthread_attr_t clock_consumer_attr;
	pthread_attr_t cpu_wast_time_attr[2];

	CHECK_SUCCESS(pthread_attr_init(&clock_generator_attr));
	CHECK_SUCCESS(pthread_attr_init(&clock_consumer_attr));
	CHECK_SUCCESS(pthread_attr_init(&cpu_wast_time_attr[0]));
	CHECK_SUCCESS(pthread_attr_init(&cpu_wast_time_attr[1]));


	// PTHREAD_EXPLICIT_SCHED to explicitly set priorities
	CHECK_SUCCESS(pthread_attr_setinheritsched(&clock_generator_attr,PTHREAD_EXPLICIT_SCHED));
	CHECK_SUCCESS(pthread_attr_setinheritsched(&clock_consumer_attr,PTHREAD_EXPLICIT_SCHED));
	CHECK_SUCCESS(pthread_attr_setinheritsched(&cpu_wast_time_attr[0],PTHREAD_EXPLICIT_SCHED));
	CHECK_SUCCESS(pthread_attr_setinheritsched(&cpu_wast_time_attr[1],PTHREAD_EXPLICIT_SCHED));

	// set scheduling parameters (priorities)
	CHECK_SUCCESS(pthread_attr_setschedparam(&clock_generator_attr,&clock_generator_param));
	CHECK_SUCCESS(pthread_attr_setschedparam(&clock_consumer_attr,&clock_consumer_param));
	CHECK_SUCCESS(pthread_attr_setschedparam(&cpu_wast_time_attr[0],&cpu_wast_time_param[0]));
	CHECK_SUCCESS(pthread_attr_setschedparam(&cpu_wast_time_attr[1],&cpu_wast_time_param[1]));

	// create threads and semaphores
	sem_t clock_sem;
	sem_t waste_cpu_time_sems[2];

	CHECK_SUCCESS(sem_init(&clock_sem,0,0));
	CHECK_SUCCESS(sem_init(&waste_cpu_time_sems[0],0,0));
	CHECK_SUCCESS(sem_init(&waste_cpu_time_sems[1],0,0));

	clock_consumer_args_t clock_consumer_args = {.clock_sem = &clock_sem, .waste_cpu_time_sems = waste_cpu_time_sems};

	CHECK_SUCCESS(pthread_create(&clock_generator, &clock_generator_attr,clock_generator_thread,&clock_sem));
	CHECK_SUCCESS(pthread_create(&clock_consumer, &clock_consumer_attr,clock_consumer_thread,&clock_consumer_args));
	CHECK_SUCCESS(pthread_create(&cpu_wast_time[0], &cpu_wast_time_attr[0],waste_cpu_time_thread,&waste_cpu_time_sems[0]));
	CHECK_SUCCESS(pthread_create(&cpu_wast_time[1], &cpu_wast_time_attr[1],waste_cpu_time_thread,&waste_cpu_time_sems[1]));

	CHECK_SUCCESS(pthread_join(clock_generator,NULL));
	CHECK_SUCCESS(pthread_join(clock_consumer,NULL));
	CHECK_SUCCESS(pthread_join(cpu_wast_time[0],NULL));
	CHECK_SUCCESS(pthread_join(cpu_wast_time[1],NULL));

	return EXIT_SUCCESS;
}

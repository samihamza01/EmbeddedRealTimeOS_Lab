/*
 * task6_stack_tracing.c
 *
 * Task 6: Sourcefile for functionalities related to stack tracing.
 *
 * Created on: Nov 26, 2023
 * Author: 	Valentin Lairich
 * Author:	Sami Hamza
 * Version:	1.0
 * Updated:	Nov 26, 2023
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <task4_cooperating_tasks.h>
#include <task6_stack_tracing.h>
#include <main.h>


#define SEQUENCE_LENGTH 2
uint8_t sequence[SEQUENCE_LENGTH] = {'H','M'};

void init_stack(void* memory_ptr, size_t size) {
	int sequence_counter = 0;

	uint8_t* ptr = (uint8_t*)memory_ptr;

	for (size_t i = 0; i < size; i++) {
		ptr[i] = sequence[sequence_counter];

		if (sequence_counter < SEQUENCE_LENGTH) {
			sequence_counter++;
		} else {
			sequence_counter = 0;
		}
	}
}

size_t get_stack_watermark(void* memory_ptr, size_t size) {
	size_t empty_byte_counter = 0;
	int sequence_counter = 0;

	uint8_t* ptr = (uint8_t*)memory_ptr;

	for (size_t i = 0; i < size; i++) {
		if (ptr[i] == sequence[sequence_counter]) {
			empty_byte_counter++;
			if (sequence_counter < SEQUENCE_LENGTH) {
				sequence_counter++;
			} else {
				sequence_counter = 0;
			}
		} else {
			break;
		}
	}

	return size-empty_byte_counter;
}

int task6_main() {
	// determined used stack sizes
	size_t stack_size_clock_generator;
	size_t stack_size_clock_consumer;
	size_t stack_size_cpu_wast_time_1;
	size_t stack_size_cpu_wast_time_2;

	// pointers to stack
	void* ptr_stack_clock_generator;
	void* ptr_stack_clock_consumer;
	void* ptr_stack_cpu_wast_time_1;
	void* ptr_stack_cpu_wast_time_2;
	size_t init_stack_size = 500000;

	// allocate and init stack memory
	ptr_stack_clock_generator = malloc(init_stack_size);
	ptr_stack_clock_consumer = malloc(init_stack_size);
	ptr_stack_cpu_wast_time_1 = malloc(init_stack_size);
	ptr_stack_cpu_wast_time_2 = malloc(init_stack_size);

	init_stack(ptr_stack_clock_generator, init_stack_size);
	init_stack(ptr_stack_clock_consumer, init_stack_size);
	init_stack(ptr_stack_cpu_wast_time_1, init_stack_size);
	init_stack(ptr_stack_cpu_wast_time_2, init_stack_size);

	// calibrate waste time function
	waste_time_calibrate();

	// create all threads: clock generator-, clock consumer- and two worker threads
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
	// printf("Main thread priority: %d\n",main_prio);
	// set priorities accordingly
	clock_generator_param.sched_priority = main_prio+2;
	clock_consumer_param.sched_priority = main_prio+1;
	cpu_wast_time_param[0].sched_priority = main_prio;
	cpu_wast_time_param[1].sched_priority = main_prio;



	// define thread attributes
	pthread_attr_t clock_generator_attr;
	pthread_attr_t clock_consumer_attr;
	pthread_attr_t cpu_wast_time_attr[2];

	CHECK_SUCCESS(pthread_attr_init(&clock_generator_attr));
	CHECK_SUCCESS(pthread_attr_init(&clock_consumer_attr));
	CHECK_SUCCESS(pthread_attr_init(&cpu_wast_time_attr[0]));
	CHECK_SUCCESS(pthread_attr_init(&cpu_wast_time_attr[1]));

	// set thread stack
	CHECK_SUCCESS(pthread_attr_setstackaddr(&clock_generator_attr,ptr_stack_clock_generator));
	CHECK_SUCCESS(pthread_attr_setstackaddr(&clock_consumer_attr,ptr_stack_clock_consumer));
	CHECK_SUCCESS(pthread_attr_setstackaddr(&cpu_wast_time_attr[0],ptr_stack_cpu_wast_time_1));
	CHECK_SUCCESS(pthread_attr_setstackaddr(&cpu_wast_time_attr[1],ptr_stack_cpu_wast_time_2));
	CHECK_SUCCESS(pthread_attr_setstacksize(&clock_generator_attr,init_stack_size));
	CHECK_SUCCESS(pthread_attr_setstacksize(&clock_consumer_attr,init_stack_size));
	CHECK_SUCCESS(pthread_attr_setstacksize(&cpu_wast_time_attr[0],init_stack_size));
	CHECK_SUCCESS(pthread_attr_setstacksize(&cpu_wast_time_attr[1],init_stack_size));

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

	// let run for 2 s
	sleep(2);

	// cancel for evaluation
	CHECK_SUCCESS(pthread_cancel(clock_generator));
	CHECK_SUCCESS(pthread_cancel(clock_consumer));
	CHECK_SUCCESS(pthread_cancel(cpu_wast_time[0]));
	CHECK_SUCCESS(pthread_cancel(cpu_wast_time[1]));

	CHECK_SUCCESS(pthread_join(clock_generator,NULL));
	CHECK_SUCCESS(pthread_join(clock_consumer,NULL));
	CHECK_SUCCESS(pthread_join(cpu_wast_time[0],NULL));
	CHECK_SUCCESS(pthread_join(cpu_wast_time[1],NULL));

	// get watermarks
	stack_size_clock_generator = get_stack_watermark(ptr_stack_clock_generator, init_stack_size);
	stack_size_clock_consumer = get_stack_watermark(ptr_stack_clock_consumer, init_stack_size);
	stack_size_cpu_wast_time_1 = get_stack_watermark(ptr_stack_cpu_wast_time_1, init_stack_size);
	stack_size_cpu_wast_time_2 = get_stack_watermark(ptr_stack_cpu_wast_time_2, init_stack_size);

	// free memory
	free(ptr_stack_clock_generator);
	free(ptr_stack_clock_consumer);
	free(ptr_stack_cpu_wast_time_1);
	free(ptr_stack_cpu_wast_time_2);

	// print result
	printf("Threads and corresponding used stack size in bytes\n");
	printf("clock generator: \t%zu bytes\n", stack_size_clock_generator);
	printf("clock consumer: \t%zu bytes\n", stack_size_clock_consumer);
	printf("waste cpu time 1: \t%zu bytes\n", stack_size_cpu_wast_time_1);
	printf("waste cpu time 2: \t%zu bytes\n", stack_size_cpu_wast_time_2);


	return	EXIT_SUCCESS;
}

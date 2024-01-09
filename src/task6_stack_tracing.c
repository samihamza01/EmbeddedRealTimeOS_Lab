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


#define SEQUENCE_LENGTH 2
uint8_t sequence[SEQUENCE_LENGTH] = {'H','M'};

void init_stack(void* memory_ptr, int size) {
	int sequence_counter = 0;

	uint8_t* ptr = (uint8_t*)memory_ptr;

	for (int i = 0; i < size; i++) {
		ptr[i] = sequence[sequence_counter];

		if (sequence_counter < SEQUENCE_LENGTH) {
			sequence_counter++;
		} else {
			sequence_counter = 0;
		}
	}
}

int get_stack_watermark(void* memory_ptr, int size) {
	int empty_byte_counter = 0;
	int sequence_counter = 0;

	uint8_t* ptr = (uint8_t*)memory_ptr;

	for (int i = 0; i < size; i++) {
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


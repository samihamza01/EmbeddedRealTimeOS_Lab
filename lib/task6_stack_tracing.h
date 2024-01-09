/*
 * task6_stack_tracing.c
 *
 * Task 6: Public inerface for functionalities related to stack tracing.
 *
 * Created on: Nov 26, 2023
 * Author: 	Valentin Lairich
 * Author:	Sami Hamza
 * Version:	1.0
 * Updated:	Nov 26, 2023
 */

/*
 * @brief Function to initialize allocated memory with an 'HM' sequence.
 * @param memory_ptr: Pointer to the memory.
 * @param size: Size of memory in bytes.
 */
void init_stack(void* memory_ptr, int size);


/*
 * @brief Function to determine the number of bytes actually used by the thread by checking the integrity of the initialized sequence.
 * @param memory_ptr: Pointer to the memory.
 * @param size: Size of memory in bytes.
 */
int get_stack_watermark(void* memory_ptr, int size);

/*
 * task2_waste_time.h
 *
 * header for task 2
 *
 * Created on: Nov 25, 2023
 * Author: 	Valentin Lairich
 * Author:	Sami Hamza
 * Version:	1.0
 * Updated:	Nov 25, 2023
 */


#ifndef TASK3_WASTE_TIME_H_
#define TASK3_WASTE_TIME_H_

// FUNCTIONS /////////////////////////////////////////////////////////////////////

/* Function to waste waste CPU time. */
void waste_time(unsigned int msecs);

/* Calibrate the waste_time function.*/
void waste_time_calibrate(void);

// @brief Function wrapper to perform specified tasks of task 3. Calls the waste_time function with several delays. These can then be analysed with the kernel trace. Should be called in main to perform the desired tasks.
// @returns Status exit code.
int task3_main(void);

#endif

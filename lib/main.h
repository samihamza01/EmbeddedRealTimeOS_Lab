/*
 * sim_main.h
 *
 * main header file for the "Embedded- und Echtzeitbetriebssysteme"
 * lab
 *
 * Created on: Nov 25, 2023
 * Author: 	Valentin Lairich
 * Author:	Sami Hamza
 * Version:	1.0
 * Updated:	Nov 25, 2023
 */

#ifndef MAIN_H_
#define MAIN_H_

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

#endif

/*
 * utils.c
 *
 *  Created on: Jul 14, 2024
 *      Author: eyalk
 */

#include "utils.h"

void printUART(char* msg) {
	printf("%s", msg);
	fflush(stdout);
}

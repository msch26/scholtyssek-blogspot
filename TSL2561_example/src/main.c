/*
 * main.c
 *
 *  Copyright (C) 2014 Marco Scholtyssek <code@scholtyssek.org>
 *
 */
#include <stdlib.h>
#include "TSL2561.h"

int main(int argc, char **argv) {
	unsigned long lux;

	init_lightsensor();

	while(1){
		lux = read_intensity();
	}
	return 0;
}

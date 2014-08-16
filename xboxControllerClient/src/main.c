/*
 * main.c
 *
 *  Copyright (C) 2014 Marco Scholtyssek <code@scholtyssek.org>
 *
 */
#include <stdlib.h>
#include "xboxController.h"

int main(int argc, char **argv) {

	if (initXboxContoller(XBOX_DEVICE) >= 0) {
		xboxCtrl* xbox = getXboxDataStruct();
		readXboxControllerInformation(xbox);

		printf("xbox controller detected\n\naxis:\t\t%d\nbuttons:\t%d\nidentifier:\t%s\n",
				xbox->numOfAxis, xbox->numOfButtons, xbox->identifier);

		while (1) {
			readXboxData(xbox);
			printXboxCtrlValues(xbox);
		}

		deinitXboxController(xbox);
	}
	return 0;
}

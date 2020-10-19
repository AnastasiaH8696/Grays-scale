/***** INCLUDES*****/

#include <stdio.h>
#include <stdlib.h>

#include "definitions.h"

/******************* Static Function Prototypes *******************/

/******************* Function Implementation *******************/

void checkMemory(void* ptr)
{
	if (ptr == NULL)
	{
		fprintf(stderr, "Allocation Failure");
		exit(ALLOCATION_ERROR);
	}
}

void checkFileOpening(void* ptr, char* fileName)
{
	if (ptr == NULL)
	{
		fprintf(stderr, "Error with opening the file: %s\n", fileName);
		exit(OPEN_FILE_ERROR);
	}
}

void checkPGMVersion(char* version)
{
	if (!strcmp(version, "P5")) {
		fprintf(stderr, "Wrong file version!\nPlease use files in P2 format\n");
		exit(FILE_VERSION_ERROR);
	}
}
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

void checkFileOpening(void* ptr)
{
	if (ptr == NULL)
	{
		fprintf(stderr, "File memory failure");
		exit(OPEN_FILE_ERROR);
	}
}
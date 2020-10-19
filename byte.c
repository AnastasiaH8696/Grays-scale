/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "definitions.h"
#include "byte.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

/******************* Function Implementation *******************/

void raiseFlag(BYTE*** flag, imgPos pos)
{
	uchar bitToRaise = 1 << (BYTE_SIZE - (pos[COLS] % BYTE_SIZE) - 1);
	(*flag)[pos[ROWS]][BYTE_SIZE - pos[COLS] / BYTE_SIZE] |= bitToRaise;
}

BOOL isFlagSet(BYTE*** flag, imgPos pos)
{
	uchar bitToCheck = 1 << (BYTE_SIZE - (pos[COLS] % BYTE_SIZE) - 1);
	return (*flag)[pos[ROWS]][BYTE_SIZE - pos[COLS] / BYTE_SIZE] & bitToCheck;
}
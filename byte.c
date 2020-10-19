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
	ushort byteToRaise = pos[COLS] / BYTE_SIZE;
	ushort bitToRaise = 1 << (BYTE_SIZE - (pos[COLS] % BYTE_SIZE) - 1);
	(*flag)[pos[ROWS]][byteToRaise] |= bitToRaise;
}

BOOL isFlagSet(BYTE*** flag, imgPos pos)
{
	ushort byteToCheck = pos[COLS] / BYTE_SIZE;
	uchar bitToCheck = 1 << (BYTE_SIZE - (pos[COLS] % BYTE_SIZE) - 1);
	return ((*flag)[pos[ROWS]][byteToCheck]) & bitToCheck;
}
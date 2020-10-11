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
BOOL isBitSet(uchar ch, int i)
{
	uchar mask = 1 << i;
	return ch & mask;
}

uchar setBit(uchar ch, int i)
{
	uchar mask = 1 << i;
	return ch | mask;
}
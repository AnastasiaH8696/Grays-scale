#ifndef ___BYTE___
#define ___BYTE___

/***** INCLUDES *****/
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/
BOOL isBitSet(uchar ch, int i);
uchar setBit(uchar ch, int i);
void raiseFlag(BYTE*** flag, imgPos pos);
BOOL isFlagSet(BYTE*** flag, imgPos pos);

#endif


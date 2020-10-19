#ifndef ___BYTE___
#define ___BYTE___

/***** INCLUDES *****/
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/

/* The function gets pointer to flag table and a specific position and sets the position from 0 to 1 */
void raiseFlag(BYTE*** flag, imgPos pos);
/* The function gets pointer to flag table and a specific position and it checks if the flag is set to 1 at this position */
BOOL isFlagSet(BYTE*** flag, imgPos pos);

#endif


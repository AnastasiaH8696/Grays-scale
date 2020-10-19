#ifndef ___SEGMENT___
#define ___SEGMENT___

/***** INCLUDES *****/
#include "definitions.h"
#include "byte.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/

/* The function finds a segment, starting at the given kernel position, limited by the value given threshold.
The function returns a pointer to the found segment. */
Segment* findSingleSegment(grayImage* img, imgPos kernel, uchar threshold);

/*Free functions*/
void freeSegmentsArr(imgPosCell** segments, ushort size);

#endif

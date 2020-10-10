#ifndef ___SEGMENT___
#define ___SEGMENT___

/***** INCLUDES *****/
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/
Segment* findSingleSegment(grayImage* img, imgPos kernel, uchar threshold);

/* The function finds a segment, starting at the given kernel, limited by the value
   given in threshold. The function returns the found segment. */
Segment* findSingleSegment(grayImage* img, imgPos kernel, unsigned char threshold);

#endif

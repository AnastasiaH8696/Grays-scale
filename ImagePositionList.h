#ifndef ___IMG_POS_LIST___
#define ___IMG_POS_LIST___

/***** INCLUDES *****/

#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/

/* This function getting a pointer to an image, threshold and segments array and it updates the segments array with values
and returns the size of the array */
uint findAllSegments(grayImage* img, unsigned char threshold,
	imgPosCell*** segments);

#endif

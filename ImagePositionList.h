#ifndef ___IMG_POS_LIST___
#define ___IMG_POS_LIST___

/***** INCLUDES *****/

#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/

uint findAllSegments(grayImage* img, unsigned char threshold,
	imgPosCell*** segments);

#endif

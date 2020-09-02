#ifndef ___IMG_POS_LIST___
#define ___IMG_POS_LIST___

/***** INCLUDES *****/

#include "GrayImage.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/***** Gray image position list data structure *****/

typedef struct _imgPosCell
{
	imgPos position;
	struct _imgPosCell* prev, * next;
} imgPosCell;

/******************* Function Prototypes *******************/


#endif

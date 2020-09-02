#ifndef ___GRAY_IMAGE___
#define ___GRAY_IMAGE___

/***** INCLUDES *****/

/***** DEFINES *****/

/***** TYPEDEFS *****/

/***** Gray image data structure *****/

typedef struct _grayImage
{
	unsigned short rows, cols;
	unsigned char** pixels;
} grayImage;

typedef unsigned short imgPos[2];
/******************* Function Prototypes *******************/


#endif

#ifndef ___GRAY_IMAGE___
#define ___GRAY_IMAGE___

/***** INCLUDES *****/
#include "definitions.h"
/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/

PIXEL** createEmptyImg(ushort rows, ushort cols);
BOOL isAllCovered(PIXEL** img, ushort cols, ushort rows);
void findMinKernel(imgPos* kernel, grayImage* img, PIXEL*** flag);
grayImage* colorSegments(grayImage* img, imgPosCell** segments, uint size);

#endif

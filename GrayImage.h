#ifndef ___GRAY_IMAGE___
#define ___GRAY_IMAGE___

/***** INCLUDES *****/
#include "definitions.h"
/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/

BYTE** createEmptyImg(ushort rows, ushort cols);
BOOL isAllCovered(BYTE** img, ushort rows, ushort cols);
void findMinKernel(imgPos* kernel, grayImage* img, BYTE*** flag);
void findNextMinValue(imgPos* kernel, grayImage* img);
grayImage* colorSegments(grayImage* img, imgPosCell** segments, uint size);

#endif

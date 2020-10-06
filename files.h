#ifndef ___FILES___
#define ___FILES___

/***** INCLUDES *****/
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/
void saveCompressed(char* fileName, grayImage* img, uchar reducedGrayLevels);
void checkFileMemory(void* ptr);

#endif


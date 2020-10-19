#ifndef ___FILES___
#define ___FILES___

/***** INCLUDES *****/
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/
grayImage* readPGM(char* fname);
void skipComments(FILE* fp);
void saveCompressed(char* fileName, grayImage* img, uchar reducedGrayLevels);
void checkFileOpening(void* ptr);
void convertCompressedImageToPGM(char* compressed_file_name, char* pgm_file_name);

#endif


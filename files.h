#ifndef ___FILES___
#define ___FILES___

/***** INCLUDES *****/
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/

/* The function gets a path to a PGM file and reads its content into a grayImage struct, and return it */
grayImage* readPGM(char* fname);

/* The function skips white spaces and comments in a given file, in a recursive way */
void skipComments(FILE* fp);

/* The function gets an output file name a grayImage structure and a number indicates the compression ratio,
	The function then compress the structure and write the compressed version into the output file */
void saveCompressed(char* fileName, grayImage* img, uchar reducedGrayLevels);

/* The function gets a path to a compressed version of a grayImage structure and writes it in a PGM version to a given output file path*/
void convertCompressedImageToPGM(char* compressed_file_name, char* pgm_file_name);

#endif


/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "byte.h"
#include "definitions.h"
#include "files.h"
#include "GrayImage.h"
#include "ImagePositionList.h"
#include "Segment.h"
#include "tests.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

/******************* Function Implementation *******************/
void e2eTest()
{
	grayImage* img = readPGM("feep.ascii.pgm");
	imgPosCell** segments;
	int numOfSegments = findAllSegments(img, 10, &segments);
	grayImage* newImg = colorSegments(img, segments, numOfSegments);
	saveCompressed("compressed.bin", newImg, 32);
}
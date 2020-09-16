/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GrayImage.h"
#include "ImagePositionList.h"
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

/******************* Function Implementation *******************/

unsigned int findAllSegments(grayImage* img, unsigned char threshold,
	imgPosCell*** segments)
{
	unsigned int size = 0;
	Segment* minSegment;
	PIXEL** flag = createEmptyImg(img); //TODO: Create empty matrix

	/*The loop is running until all the segments are covered by the flag*/
	while (!isAllCovered(flag)) //TODO: Check if flag values are all equals 1
	{
		/*Finding minimum segment*/
		minSegment = findMinSegment(img, threshold, flag); //TODO: Find segment & mark it on the flag.

		/*Adding the segment to the array*/
		addSegmentToArray(minSegment, segments, flag); //TODO: Add segment & sort it.
		size++;
	}

	return size;
}
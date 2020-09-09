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
	short int minSegmentRowPos, minSegmentColPos;
	unsigned char minPixel;

	/*Finding the maximum segment and the minimum from pixel 0 */
	Segment* maxSegment = findMaxSegment(img); //TODO
	Segment* minSegment = findMinSegment(0, img); //TODO

	while (maxSegment->root->position != minSegment->root->position)
	{
		/*In this function we will add the segment in the proper order*/
		addSegmentToArray(minSegment, segments); //TODO
		size++;

		minSegmentRowPos = minSegment->root->position[0];
		minSegmentColPos = minSegment->root->position[1];
		/*Getting the value of the current minimum pixel*/
		minPixel = findPixel(minSegmentRowPos, minSegmentColPos); //TODO
		
		/*Searching for the next minimum segment from the prev minimum pixels value plus one*/
		minSegment = findMinSegment((minPixel - '0') + 1, img);
	}

	return size;
}
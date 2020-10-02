/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GrayImage.h"
#include "ImagePositionList.h"
#include "Segment.h"
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

static imgPosCell* sortNeighbors(Segment* segment, PIXEL*** flag);
static void addSegmentToArray(Segment* segment, imgPosCell*** segments, 
	uint size, uint segmentSize);

/******************* Function Implementation *******************/

uint findAllSegments(grayImage* img, unsigned char threshold,
	imgPosCell*** segments)
{
	uint size = 0, segmentSize;
	Segment* minSegment;
	imgPos kernel;
	imgPosCell* curr;
	/*Creating a copy of our image with zeroes for tracking*/
	PIXEL** flag = createEmptyImg(img->rows, img->cols); 
	

	/*The loop is running until all the segments are covered by the flag*/
	while (!isAllCovered(flag, img->cols, img->rows))
	{
		/*Finding segment and adding it to the imgPosList sorted by location*/
		findMinKernel(kernel, img, flag);
		minSegment = findSingleSegment(img, kernel, threshold);
		curr = sortNeighbors(minSegment, flag); //TODO: Sort the segment by the locations and create ImgPosCell list
		segmentSize = minSegment->size;

		/*Adding the segment to the array*/
		addSegmentToArray(curr, segments, size, segmentSize); //TODO: Add segment & sort it by size of single segment.
		size++;
	}

	return size;
}

imgPosCell* sortNeighbors(Segment* segment, PIXEL*** flag)
{
	//TODO
}

static void addSegmentToArray(Segment* segment, imgPosCell*** segments,
	uint size, uint segmentSize)
{
	//TODO
}
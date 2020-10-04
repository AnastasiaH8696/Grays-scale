/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GrayImage.h"
#include "Segment.h"
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

/******************* Function Implementation *******************/

Segment* findSingleSegment(grayImage* img, imgPos kernel, uchar threshold)
{
	Segment res_segment;
	addSimilarNeighbors(img, kernel, threshold, res_segment);
	return &res_segment;
}

void addSimiliarNeighbors(grayImage* img, imgPos pos, unsigned char threshold, Segment seg)
{
	addSimiliarNeighborsRec(img, pos, threshold, seg.root);
}

void addSimiliarNeighborsRec(grayImage* img, imgPos pos, unsigned char threshold, treeNode* node)
{
	if (isCovered(pos) || !inThreshold(pos))
		return;

	for (unsigned int i = -1; i <= 1; i++)
	{
		for (unsigned int j = -1; j <= 1; j++)
		{
			pos[ROWS] += i;
			pos[COLS] += j;
			addSimilarNeighborsRec();
		}
	}
}
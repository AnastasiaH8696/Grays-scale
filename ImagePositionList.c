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
static void sortSegments(grayImage* img, uchar threshold, imgPosCell*** segments, uint size);
static void addToBeginningOfList(imgPosCell** nodes, imgPos* position);
static void bubbleSort(imgPosCell* nodes);
static BOOL isBigger(imgPosCell* maxNode, imgPosCell* minNode);
static void swap(imgPosCell* maxNode, imgPosCell* minNode);

/******************* Function Implementation *******************/

uint findAllSegments(grayImage* img, unsigned char threshold,
	imgPosCell*** segments)
{
	uint size = 0, physize = 1;
	Segment* minSegment;
	imgPos kernel;
	imgPosCell* curr;
	/*Creating a copy of our image with zeroes for tracking*/
	PIXEL** flag = createEmptyImg(img->rows, img->cols); 

	segments = (imgPosCell***)malloc(sizeof(imgPosCell**) * physize);
	checkMemory(segments);
	

	/*The loop is running until all the segments are covered by the flag*/
	while (!isAllCovered(flag, img->cols, img->rows))
	{
		/*Finding segment and adding it to the imgPosList sorted by location*/
		findMinKernel(kernel, img, flag);
		minSegment = findSingleSegment(img, kernel, threshold);
		curr = sortNeighbors(minSegment, flag); 

		/*Adding the segment to the array*/
		if (size == physize)
		{
			physize *= 2;
			segments = (imgPosCell***)realloc(segments, sizeof(imgPosCell**)*physize);
			checkMemory(segments);
		}
		*segments[size] = curr;
		size++;
	}

	segments = (imgPosCell***)realloc(segments, sizeof(imgPosCell**) * size);
	checkMemory(segments);

	sortSegments(img, threshold, segments, size);
	return size;
}

static imgPosCell* sortNeighbors(Segment* segment, PIXEL*** flag)
{
	imgPosCell* nodes;
	ushort i, size = segment->size;

	addToBeginningOfList(&nodes, segment->root->position);
	for (i = 1; i < size; i++)
		addToBeginningOfList(&nodes, segment->root->similar_neighbors[i-1]->position);
	
	bubbleSort(nodes);

	return nodes;
}

static void addToBeginningOfList(imgPosCell** nodes, imgPos* position, PIXEL*** flag)
{
	imgPosCell* node = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkMemory(node);

	node->position[0] = position[0];
	node->position[1] = position[1];
	node->next = *nodes;
	*nodes = node;
	node->prev = NULL;

	flag[node->position[0]][node->position[1]] = 1;
}

static void bubbleSort(imgPosCell *nodes)
{
	BOOL isSorted = FALSE;
	imgPosCell* curr = nodes;
	imgPos tmp;
	while (!isSorted)
	{
		isSorted = TRUE;
		curr = nodes;
		while (curr->next != NULL)
		{
			if (isBigger(curr->position, curr->next->position))
			{
				isSorted = FALSE;
				swap(curr, curr->next);
			}
			curr = curr->next;
		}
	}
}

static BOOL isBigger(imgPosCell* maxNode, imgPosCell* minNode)
{
	BOOL flag = FALSE;

	if (maxNode->position[0] > minNode->position[0])
		flag = TRUE;
	else if ((maxNode->position[0] == minNode->position[0])
		&& maxNode->position[1] > minNode->position[1])
		flag = TRUE;

	return flag;
}

static void swap(imgPosCell* maxNode, imgPosCell* minNode)
{
	imgPos tmp;
	tmp[0] = maxNode->position[0];
	tmp[1] = maxNode->position[1];

	maxNode->position[0] = minNode->position[0];
	maxNode->position[1] = minNode->position[1];

	minNode->position[0] = tmp[0];
	minNode->position[1] = tmp[1];
}

static void sortSegments(grayImage* img, uchar threshold, imgPosCell*** segments, uint size)
{
	ushort i, j;
	Segment* curr;
	Segment *next;
	imgPosCell* temp;

	for (i = 0; i < size - 1; i++)
	{
		for (j = 0; j < size - i - 1; j++)
		{
			curr = findSingleSegment(img, (*segments)[j]->position, threshold);
			next = findSingleSegment(img, (*segments)[j+1]->position, threshold);

			if (curr->size < next->size)
			{
				temp = (*segments)[j];
				(*segments)[j] = (*segments)[j + 1];
				(*segments)[j + 1] = temp;
			}
		}
	}
}
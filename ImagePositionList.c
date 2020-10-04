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
static void addToBeginningOfList(imgPosCell** nodes, imgPos* position, PIXEL ***flag);
static void bubbleSort(imgPosCell* nodes);
static BOOL isBigger(imgPosCell* maxNode, imgPosCell* minNode);
static void swap(imgPosCell* maxNode, imgPosCell* minNode);
static void addItemToArray(ushort* size, ushort* physize, imgPosCell**** segments, imgPosCell* curr);
static void freeflag(PIXEL** flag, ushort rows);
static void freeSegmentsArr(imgPosCell** segments, ushort size);
static void deleteFromBeginning(imgPosCell* node);

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
	/*Allocating memory for segments array*/
	*segments = (imgPosCell*)malloc(sizeof(imgPosCell) * physize);
	checkMemory(*segments);
	
	/*The loop is running until all the segments are covered by the flag*/
	while (!isAllCovered(flag, img->cols, img->rows))
	{
		/*Finding segment and adding it to the imgPosList sorted by location*/
		findMinKernel(&kernel, img, &flag);
		minSegment = findSingleSegment(img, kernel, threshold);
		curr = sortNeighbors(minSegment, &flag); 

		/*Adding the segment to the array*/
		addItemToArray(&size, &physize, &segments, curr);
	}

	/*Reallocate to the right size*/
	if (size < physize)
	{
		*segments = (imgPosCell*)realloc(segments, sizeof(imgPosCell) * size);
		checkMemory(*segments);
	}

	sortSegments(img, threshold, segments, size);
	freeflag(flag, img->rows);
	return size;
}

static imgPosCell* sortNeighbors(Segment* segment, PIXEL*** flag)
{
	imgPosCell* nodes = NULL;
	ushort i, size = segment->size;

	addToBeginningOfList(&nodes, segment->root->position, flag);
	for (i = 0; i < size; i++)
		addToBeginningOfList(&nodes, segment->root->similar_neighbors[i]->position, flag);
	
	bubbleSort(nodes);

	return nodes;
}

static void addToBeginningOfList(imgPosCell** nodes, imgPos* position, PIXEL*** flag)
{
	imgPosCell* node;
	node = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkMemory(node);

	*node->position = position;
	node->next = *nodes;
	node->prev = NULL;
	(*nodes)->prev = node;
	*nodes = node;
	
	*flag[node->position[0]][node->position[1]] = 1;
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

static void addItemToArray(ushort* size, ushort* physize,imgPosCell**** segments,imgPosCell* curr)
{
	if (*size == *physize)
	{
		*physize *= 2;
		**segments = (imgPosCell**)realloc(segments, sizeof(imgPosCell*) * (*physize));
		checkMemory(**segments);
	}
	**segments[*size] = curr;
	(*size)++;
}

static void freeflag(PIXEL** flag, ushort rows)
{
	ushort i;
	for (i = 0; i < rows; i++)
		free(flag[i]);
	free(flag);
}

static void freeSegmentsArr(imgPosCell** segments, ushort size)
{
	ushort i;
	for (i = 0; i < size; i++)
		while (segments[i])
			deleteFromBeginning(segments[i]);
	free(segments);
}

static void deleteFromBeginning(imgPosCell* node)
{
	node = node->next;
	free(node->prev);
	node->prev = NULL;
}
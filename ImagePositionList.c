/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GrayImage.h"
#include "ImagePositionList.h"
#include "Segment.h"
#include "definitions.h"
#include "byte.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

static imgPosCell* sortNeighbors(Segment* segment, BYTE*** flag);
static void sortSegments(grayImage* img, uchar threshold, imgPosCell*** segments, uint size);
static void addToEmptyList(imgPosCellList* nodes, imgPos position, BYTE*** flag);
static void addToList(imgPosCellList* nodes, imgPos position, BYTE ***flag);
static BOOL isBigger(imgPosCell* maxNode, imgPosCell* minNode);
static void addItemToArray(ushort* size, ushort* physize, imgPosCell**** segments, imgPosCell* curr);
static void freeflag(BYTE** flag, ushort rows);
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
	BYTE** flag = createEmptyImg(img->rows, (img->cols)/ BYTE_SIZE); 
	/*Allocating memory for segments array*/
	*segments = (imgPosCell*)realloc(segments, sizeof(imgPosCell) * physize);
	checkMemory(*segments);
	
	/*The loop is running until all the segments are covered by the flag*/
	while (!isAllCovered(flag, img->cols, (img->rows)/ BYTE_SIZE))
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

static imgPosCell* sortNeighbors(Segment* segment, BYTE*** flag)
{
	imgPosCellList* lst;
	addToEmptyList(&lst, segment->root->position, flag);

	ushort i, size = segment->size;
	for (i = 0; i < size; i++)
		addToList(&lst, segment->root->similar_neighbors[i]->position, flag);
	
	return lst->head;
}

static void addToEmptyList(imgPosCellList* nodes, imgPos position, BYTE*** flag)
{
	imgPosCell* node;
	node = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkMemory(node);
	node->position[0] = position[0];
	node->position[1] = position[1];
	node->next = node->prev = NULL;

	*flag[node->position[0]][node->position[1] / BYTE_SIZE] = setBit(*flag[node->position[0]][node->position[1] / BYTE_SIZE],
		node->position[1] / BYTE_SIZE);
}

static void addToList(imgPosCellList* nodes, imgPos position, BYTE*** flag)
{
	imgPosCell* node;
	node = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkMemory(node);
	node->position[0] = position[0];
	node->position[1] = position[1];

	imgPosCell* curr = nodes->head, * prev;
	while (curr && isBigger(node, curr))
		curr = curr->next;

	prev = curr->prev;

	node->next = prev->next;
	prev->next = node;
	node->prev = prev;
	node->next->prev = node;
	
	*flag[node->position[0]][node->position[1] / BYTE_SIZE] = setBit(*flag[node->position[0]][node->position[1] / BYTE_SIZE],
		node->position[1] / BYTE_SIZE);
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

static void sortSegments(grayImage* img, uchar threshold, imgPosCell*** segments, uint size)
{
	ushort i, j;
	Segment* curr;
	Segment *next;
	imgPosCell* temp;
	BOOL swapped;

	for (i = 0; i < size - 1; i++)
	{
		swapped = FALSE;
		for (j = 0; j < size - i - 1; j++)
		{
			curr = findSingleSegment(img, (*segments)[j]->position, threshold);
			next = findSingleSegment(img, (*segments)[j+1]->position, threshold);

			if (curr->size < next->size)
			{
				temp = (*segments)[j];
				(*segments)[j] = (*segments)[j + 1];
				(*segments)[j + 1] = temp;
				swapped = TRUE;
			}
		}
		if (swapped == FALSE)
			break;
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

static void freeflag(BYTE** flag, ushort rows)
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
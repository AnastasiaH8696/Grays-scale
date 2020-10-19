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

/*Sorting functions*/
static imgPosCell* sortNeighbors(Segment* segment, BYTE*** flag);
static void sortSegments(grayImage* img, uchar threshold, imgPosCell*** segments, uint size, uint* sizesArr);

/*List basic functions*/
static imgPosCellList makeEmptyList();
static void addToBeginningOfList(imgPosCellList* nodes, imgPosCell* node);
static void addToEndOfList(imgPosCellList* nodes, imgPosCell* node);
static void addToInnerPlaceInList(imgPosCell* prev, imgPosCell* node);
static void addToEmptyList(imgPosCellList* nodes, imgPos position, BYTE*** flag);
static void addToListRec(imgPosCellList* nodes, treeNode* node, BYTE*** flag);
static void addToList(imgPosCellList* nodes, imgPos position, BYTE ***flag);
static void deleteFromBeginning(imgPosCell* node);

/*Array functions*/
static void addItemToArray(uint* size, uint* physize, imgPosCell*** segments, imgPosCell* curr,
	uint** sizesArr, uint segmentSize);

/*Other functions*/
static BOOL isBigger(imgPosCell* maxNode, imgPosCell* minNode);

/******************* Function Implementation *******************/

uint findAllSegments(grayImage* img, unsigned char threshold,
	imgPosCell*** segments)
{
	uint size = 0, physize = 1;
	Segment* minSegment;
	imgPos kernel = { 0,0 };
	imgPosCell* curr;
	/*Creating array for sizes*/
	uint* sizesArr = (uint*)malloc(sizeof(uint) * physize);
	checkMemory(*sizesArr);
	/*Creating a copy of our image with zeroes for tracking*/
	BYTE** flag = createEmptyImg(img->rows, (img->cols)/ BYTE_SIZE); 
	/*Allocating memory for segments array*/
	*segments = (imgPosCell**)malloc(sizeof(imgPosCell*) * physize);
	checkMemory(*segments);
	
	/*The loop is running until all the segments are covered by the flag*/
	while (!isAllCovered(&flag, img->rows, img->cols))
	{
		/*Finding segment and adding it to the imgPosList sorted by location*/
		findMinKernel(&kernel, img, &flag);
		minSegment = findSingleSegment(img, kernel, threshold);
		curr = sortNeighbors(minSegment, &flag); 
		/*Adding the segment to the array*/
		addItemToArray(&size, &physize, segments, curr, &sizesArr, minSegment->size);
		free(minSegment->root);
		free(minSegment);
	}

	/*Reallocate to the right size*/
	if (size < physize)
	{
		*segments = (imgPosCell**)realloc(*segments, sizeof(imgPosCell*) * size);
		checkMemory(*segments);
	}

	sortSegments(img, threshold, segments, size, sizesArr);
	freeflag(flag, img->rows);
	free(sizesArr);
	return size;
}

static imgPosCell* sortNeighbors(Segment* segment, BYTE*** flag)
{
	imgPosCellList lst = makeEmptyList();
	addToEmptyList(&lst, segment->root->position, flag);

	ushort i = 0;
	while (segment->root->similar_neighbors[i])
	{
		addToListRec(&lst, segment->root->similar_neighbors[i], flag);
		i++;
	}
	
	return lst.head;
}

static void addToListRec(imgPosCellList* nodes, treeNode* node, BYTE*** flag)
{
	ushort i = 0;
	if (node && (!isFlagSet(flag, node->position)))
	{
		addToList(nodes, node->position, flag);

		while (node->similar_neighbors[i])
		{
			addToListRec(nodes, node->similar_neighbors[i], flag);
			i++;
		}
	}
}

static imgPosCellList makeEmptyList()
{
	imgPosCellList lst;
	lst.head = lst.tail = NULL;
	return lst;
}


static void addToEmptyList(imgPosCellList* nodes, imgPos position, BYTE*** flag)
{
	imgPosCell* node;
	node = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkMemory(node);
	(node->position)[ROWS] = position[ROWS];
	(node->position)[COLS] = position[COLS];
	node->next = node->prev = NULL;
	nodes->head = nodes->tail = node;

	raiseFlag(flag, node->position);
}

static void addToList(imgPosCellList* nodes, imgPos position, BYTE*** flag)
{
	imgPosCell* node;
	node = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkMemory(node);
	(node->position)[ROWS] = position[ROWS];
	(node->position)[COLS] = position[COLS];

	imgPosCell* curr = nodes->head, *prev = NULL;
	while (curr && isBigger(node, curr))
	{
		prev = curr;
		curr = curr->next;
	}

	if (!prev)
		addToBeginningOfList(nodes, node);

	else if (!curr)
		addToEndOfList(nodes, node);
	else
		addToInnerPlaceInList(prev, node);

	raiseFlag(flag, node->position);
}

static void addToBeginningOfList(imgPosCellList* nodes, imgPosCell* node)
{
	node->next = nodes->head;
	nodes->head->prev = node;
	nodes->head = node;
	nodes->head->prev = NULL;
}

static void addToEndOfList(imgPosCellList* nodes, imgPosCell* node)
{
	node->prev = nodes->tail;
	nodes->tail->next = node;
	nodes->tail = node;
	nodes->tail->next = NULL;
}

static void addToInnerPlaceInList(imgPosCell* prev, imgPosCell* node)
{
	node->next = prev->next;
	prev->next = node;
	node->prev = prev;
	node->next->prev = node;
}

static BOOL isBigger(imgPosCell* maxNode, imgPosCell* minNode)
{
	BOOL flag = FALSE;

	if (maxNode->position[ROWS] > minNode->position[ROWS])
		flag = TRUE;
	else if ((maxNode->position[ROWS] == minNode->position[ROWS])
		&& maxNode->position[COLS] > minNode->position[COLS])
		flag = TRUE;

	return flag;
}

static void sortSegments(grayImage* img, uchar threshold, imgPosCell*** segments, uint size, uint* sizesArr)
{
	ushort i, j;
	imgPosCell* temp;
	BOOL swapped;

	for (i = 0; i < size - 1; i++)
	{
		swapped = FALSE;
		for (j = 0; j < size - i - 1; j++)
		{
			if (sizesArr[j] < sizesArr[j+1])
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

static void addItemToArray(uint* size, uint* physize,imgPosCell*** segments,imgPosCell* curr,
	uint **sizesArr, uint segmentSize)
{
	if (*size == *physize)
	{
		*physize *= 2;
		*segments = (imgPosCell**)realloc(*segments, *physize * sizeof(imgPosCell*));
		checkMemory(*segments);
		*sizesArr = (uint*)realloc(*sizesArr, *physize * sizeof(uint));
		checkMemory(*sizesArr);
	}
	(*segments)[*size] = curr;
	(*sizesArr)[*size] = segmentSize;
	(*size)++;
}

void freeSegmentsArr(imgPosCell** segments, ushort size)
{
	ushort i;
	for (i = 0; i < size; i++)
		while (segments[i])
			deleteFromBeginning(segments[i]);
	free(segments);
}

static void deleteFromBeginning(imgPosCell* node)
{
	imgPosCell* nodeToDel;
	while (node)
	{
		nodeToDel = node;
		node = node->next;
		node->prev = NULL;
		free(nodeToDel);
	}
}

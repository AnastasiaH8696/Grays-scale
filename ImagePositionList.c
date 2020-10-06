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
static void addToBeginningOfList(imgPosCell** nodes, imgPos* position, BYTE ***flag);
static imgPosCell* mergeSort(imgPosCell* head);
static imgPosCell* split(imgPosCell* head);
static imgPosCell* merge(imgPosCell* first, imgPosCell* second);
//static void bubbleSort(imgPosCell* nodes);
static BOOL isBigger(imgPosCell* maxNode, imgPosCell* minNode);
//static void swap(imgPosCell* maxNode, imgPosCell* minNode);
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
	BYTE** flag = createEmptyImg(img->rows, (img->cols)/8); 
	/*Allocating memory for segments array*/
	*segments = (imgPosCell*)malloc(sizeof(imgPosCell) * physize);
	checkMemory(*segments);
	
	/*The loop is running until all the segments are covered by the flag*/
	while (!isAllCovered(flag, img->cols, (img->rows)/8))
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
	imgPosCell* nodes = NULL;
	ushort i, size = segment->size;

	addToBeginningOfList(&nodes, segment->root->position, flag);
	for (i = 0; i < size; i++)
		addToBeginningOfList(&nodes, segment->root->similar_neighbors[i]->position, flag);
	
	nodes = mergeSort(nodes);

	return nodes;
}

static void addToBeginningOfList(imgPosCell** nodes, imgPos* position, BYTE*** flag)
{
	imgPosCell* node;
	node = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkMemory(node);

	*node->position = position;
	node->next = *nodes;
	node->prev = NULL;
	(*nodes)->prev = node;
	*nodes = node;
	
	*flag[node->position[0]][node->position[1] / 8] = setBit(*flag[node->position[0]][node->position[1] / 8],
		node->position[1] / 8);
}

/* Function to do merge sort */
static imgPosCell* mergeSort(imgPosCell* head)
{
	if (!head || !head->next)
		return head;
	imgPosCell* second = split(head);

	/*Recur for left and right halves*/ 
	head = mergeSort(head);
	second = mergeSort(second);

	/*Merge the two sorted halves */
	return merge(head, second);
}

/* Split a doubly linked list (DLL) into 2 DLLs of half sizes */
static imgPosCell* split(imgPosCell* head)
{
	imgPosCell* fast = head, * slow = head;
	while (fast->next && fast->next->next)
	{
		fast = fast->next->next;
		slow = slow->next;
	}
	imgPosCell* temp = slow->next;
	slow->next = NULL;
	return temp;
}

/* Function to merge two linked lists */
static imgPosCell* merge(imgPosCell* first, imgPosCell* second)
{
	/* If first linked list is empty */
	if (!first)
		return second;

	/* If second linked list is empty */
	if (!second)
		return first;

	/* Pick the smaller value */
	if (!isBigger(first->position, second->position))
	{
		first->next = merge(first->next, second);
		first->next->prev = first;
		first->prev = NULL;
		return first;
	}
	else
	{
		second->next = merge(first, second->next);
		second->next->prev = second;
		second->prev = NULL;
		return second;
	}
}

/*static void bubbleSort(imgPosCell *nodes)
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
}*/

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

/*static void swap(imgPosCell* maxNode, imgPosCell* minNode)
{
	imgPos tmp;
	tmp[0] = maxNode->position[0];
	tmp[1] = maxNode->position[1];

	maxNode->position[0] = minNode->position[0];
	maxNode->position[1] = minNode->position[1];

	minNode->position[0] = tmp[0];
	minNode->position[1] = tmp[1];
}*/

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
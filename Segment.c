/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GrayImage.h"
#include "Segment.h"
#include "definitions.h"
#include "byte.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

/******************* Function Implementation *******************/

Segment* findSingleSegment(grayImage* img, imgPos kernel, uchar threshold)
{
	Segment* resSeg = initSegment(kernel); /* The returned result segment */
	/*Creating a copy of our image with zeroes for tracking*/
	BYTE** flag = createEmptyImg(img->rows, (img->cols) / BYTE_SIZE);

	buildSegment(img, resSeg, threshold, &flag);

	return resSeg;
}

treeNode** allocateTreeNodeList()
{
	treeNode** resTreeNodeList = (treeNode**)malloc(sizeof(treeNode*));
	checkMemory(resTreeNodeList);
	resTreeNodeList[0] = NULL;
	
	return resTreeNodeList;
}

treeNode* initTreeNode(imgPos rootPos)
{
	treeNode* resTreeNode = (treeNode*)malloc(sizeof(treeNode));
	checkMemory(resTreeNode);
	resTreeNode->position[ROWS] = rootPos[ROWS];
	resTreeNode->position[COLS] = rootPos[COLS];
	resTreeNode->similar_neighbors = allocateTreeNodeList();
	
	return resTreeNode;
}

Segment* initSegment(imgPos rootPos)
{
	Segment* resSeg = (Segment*)malloc(sizeof(Segment));
	checkMemory(resSeg);
	resSeg->root = initTreeNode(rootPos);
	resSeg->size = 1;

	return resSeg;
}

void appendChild(treeNode*** tnlst, treeNode* child)
{
	ushort i = 0;
	while ((*tnlst)[i++]);
	*tnlst = (treeNode**)realloc(*tnlst, sizeof(treeNode*) * (i + 1));
	checkMemory(tnlst);
	(*tnlst)[i - 1] = child;
	(*tnlst)[i] = NULL;
}

void appendNeighbor(treeNode* tn, treeNode* neighbor)
{
	appendChild(&(tn->similar_neighbors), neighbor);
}

void addChildren(grayImage* img, treeNode* tn, uchar minVal, uchar maxVal, BYTE*** flag, uint* size)
{
	short i, j;
	imgPos potentialNeighborPos;
	treeNode* potentialNeighbor;
	uchar potentialNeighborVal;
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			potentialNeighborPos[ROWS] = tn->position[ROWS] + i;
			potentialNeighborPos[COLS] = tn->position[COLS] + j;
			potentialNeighborVal = img->pixels[potentialNeighborPos[ROWS]][potentialNeighborPos[COLS]];
			if ((minVal <= potentialNeighborVal && maxVal >= potentialNeighborVal) && !isFlagSet(flag, potentialNeighborPos))
			{
				potentialNeighbor = initTreeNode(potentialNeighborPos);
				appendNeighbor(tn, potentialNeighbor);
				raiseFlag(flag, potentialNeighborPos);
				(*size)++;
			}
		}
	}
}

void buildSegmentRec(grayImage* img, treeNode** tnlst, uchar minVal, uchar maxVal, BYTE*** flag, uint* size)
{
	ushort i = 0;
	while (tnlst[i])
	{
		addChildren(img, tnlst[i], minVal, maxVal, flag, size);
		i++;
	}
	i = 0;
	while (tnlst[i])
	{
		buildSegmentRec(img, tnlst[i]->similar_neighbors, minVal, maxVal, flag, size);
		i++;
	}
}

void buildSegment(grayImage* img, Segment* seg, uchar threshold, BYTE*** flag)
{
	uchar pixelVal = img->pixels[seg->root->position[ROWS]][seg->root->position[COLS]];
	uchar minVal = threshold > pixelVal ? MIN_VAL : pixelVal - threshold;
	uchar maxVal = threshold > MAX_VAL - pixelVal ? MAX_VAL : pixelVal + threshold;

	treeNode** tnlst = allocateTreeNodeList();
	appendChild(&tnlst, seg->root);
	raiseFlag(flag, seg->root->position);

	buildSegmentRec(img, tnlst, minVal, maxVal, flag, &(seg->size));
}

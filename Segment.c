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
	Segment res_segment;  /* The returned Segment value */

	/*Creating a copy of our image with zeroes for tracking*/
	BYTE** flag = createEmptyImg(img->rows, (img->cols) / BYTE_SIZE);

	treeNode root = createNewTreeNode(kernel);
	res_segment.root = &root;
	res_segment.size = 1;

	addSimilarNeighbors(img, kernel, threshold, res_segment);
	return &res_segment;
}

void addSimiliarNeighbors(grayImage* img, imgPos pos, unsigned char threshold, Segment seg)
{
	uchar minVal = img->pixels[pos[ROWS], pos[COLS]] - threshold;
	uchar maxVal = img->pixels[pos[ROWS], pos[COLS]] + threshold;
	addSimiliarNeighborsRec(img, pos, minVal, maxVal, seg.root);
}

void addSimiliarNeighborsRec(grayImage* img, imgPos pos, uchar minVal, uchar maxVal, treeNode** nodes)
{
	ushort i;
	if (nodes[0] == NULL)
		return;
	for (i = 0; nodes[i]; i++) {
		findChildren(img, pos, minVal, maxVal, nodes[i]);
	}
	for (i = 0; nodes[i]; i++) {
		addSimiliarNeighborsRec(img, pos, minVal, maxVal, nodes[i]->similar_neighbors);
	}
}

void findChildren(grayImage* img, imgPos pos, uchar minVal, uchar maxVal, treeNode** children)
{
	uchar neighborVal;
	imgPos neighborPos;

	/* Going over surroinding pixels */
	for (uint i = -1; i <= 1; i++)
	{
		for (uint j = -1; j <= 1; j++)
		{
			neighborPos[ROWS] = pos[ROWS] + i;
			neighborPos[COLS] = pos[COLS] + j;
			neighborVal = img->pixels[neighborPos[ROWS]][neighborPos[COLS]];
			if (!isCovered(neighborPos) && (neighborVal >= minVal && neighborVal <= maxVal))
			{
				treeNode neighbor = createNewTreeNode(neighborPos);
				appendChild(children, &neighbor);
			}
		}
	}
}

treeNode createNewTreeNode(imgPos pos)
{
	treeNode newTreeNode;
	newTreeNode.position[ROWS] = pos[ROWS];
	newTreeNode.position[COLS] = pos[COLS];
	newTreeNode.similar_neighbors = NULL;

	return newTreeNode;
}

void appendChild(treeNode** children, treeNode* child)
{
	ushort i = 0;
	while (children[i++]); /* Get to the end of the list */
	children = (treeNode *)realloc(sizeof(treeNode *), i+1);
	children[i - 1] = child;
	children[i] = NULL;
}
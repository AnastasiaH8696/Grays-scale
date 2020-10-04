#ifndef ___SEGMENT___
#define ___SEGMENT___

/***** INCLUDES *****/
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/

/* The function finds a segment, starting at the given kernel, limited by the value
given in threshold. The function returns the found segment. */
Segment* findSingleSegment(grayImage* img, imgPos kernel, uchar threshold);

void addSimiliarNeighbors(grayImage* img, imgPos pos, unsigned char threshold, Segment seg);

void addSimiliarNeighborsRec(grayImage* img, imgPos pos, uchar minVal, uchar maxVal, treeNode** nodes);

void findChildren(grayImage* img, imgPos pos, uchar minVal, uchar maxVal, treeNode** children);

treeNode createNewTreeNode(imgPos pos);

void appendChild(treeNode** children, treeNode* child);

#endif

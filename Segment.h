#ifndef ___SEGMENT___
#define ___SEGMENT___

/***** INCLUDES *****/
#include "definitions.h"
#include "byte.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Function Prototypes *******************/

/* The function finds a segment, starting at the given kernel position, limited by the value given threshold.
The function returns a pointer to the found segment. */
Segment* findSingleSegment(grayImage* img, imgPos kernel, uchar threshold);

treeNode** allocateTreeNodeList();
treeNode* initTreeNode(imgPos rootPos);
Segment* initSegment(imgPos rootPos);
void appendChild(treeNode*** tnlst, treeNode* child);
void appendNeighbor(treeNode* tn, treeNode* neighbor);
void addChildren(grayImage* img, treeNode* tn, uchar minVal, uchar maxVal, BYTE*** flag, uint* size);
void buildSegmentRec(grayImage* img, treeNode** tnlst, uchar minVal, uchar maxVal, BYTE*** flag, uint* size);
void buildSegment(grayImage* img, Segment* seg, uchar threshold, BYTE*** flag);


#endif

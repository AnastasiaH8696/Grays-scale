#ifndef ___SEGMENT___
#define ___SEGMENT___

/***** INCLUDES *****/

#include "GrayImage.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/***** Segment data structure *****/

typedef struct _treeNode
{
	imgPos position;
	struct _treeNode** similar_neighbors;
}treeNode;

typedef struct _segment
{
	treeNode* root;
	unsigned int size;
}Segment;

/******************* Function Prototypes *******************/


#endif

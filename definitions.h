#ifndef ___DEFINITIONS___
#define ___DEFINITIONS___

/***** INCLUDES *****/

/***** DEFINES *****/

/***** TYPEDEFS *****/

/**** Boolean typedef *****/

typedef short BOOL;

/**** Pixel typedef *****/

typedef unsigned char PIXEL;

/***** Gray image data structure *****/

typedef struct _grayImage
{
	unsigned short rows, cols;
	PIXEL** pixels;
} grayImage;

/***** Image position typedef *****/

typedef unsigned short imgPos[2];

/***** Gray image position list data structure *****/

typedef struct _imgPosCell
{
	imgPos position;
	struct _imgPosCell* prev, * next;
} imgPosCell;

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

void checkMemory(void* ptr);

#endif

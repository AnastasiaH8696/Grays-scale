#ifndef ___DEFINITIONS___
#define ___DEFINITIONS___

/***** INCLUDES *****/

/***** DEFINES *****/

#define FALSE 0
#define TRUE 1
#define BYTE_SIZE 8
#define MAX_PIXEL 255

/***** TYPEDEFS *****/

/**** unsigned typedefs *****/

typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char uchar;

/**** Boolean typedef *****/

typedef short BOOL;

/**** Byte typedef *****/

typedef unsigned char BYTE;

/**** Pixel typedef *****/

typedef unsigned char PIXEL;

/***** Gray image data structure *****/

typedef struct _grayImage
{
	ushort rows, cols;
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
	uint size;
}Segment;

/******************* Function Prototypes *******************/

void checkMemory(void* ptr);

#endif

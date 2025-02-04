#ifndef ___DEFINITIONS___
#define ___DEFINITIONS___

/***** INCLUDES *****/

/***** DEFINES *****/
/* Errors defines */
#define ALLOCATION_ERROR -1
#define OPEN_FILE_ERROR -2
#define FILE_VERSION_ERROR -3

/* imgPos defines */
#define ROWS 0
#define COLS 1

#define FALSE 0
#define TRUE 1
#define BYTE_SIZE 8
#define MAX_PIXEL 255
#define MIN_VAL 0
#define MAX_VAL 255

#define DEFAULT_THRESHOLD 10
#define DEFAULT_REDUCED_GRAYSCALE 32
#define IMAGES_PATH ".\\images\\"

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

typedef struct _imgPosCellList
{
	imgPosCell* head, *tail;
}imgPosCellList;

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

/* Memory allocation check */
void checkMemory(void* ptr);
void checkFileOpening(void* ptr, char* fileName);
void checkPGMVersion(char* version);

#endif

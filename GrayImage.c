/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GrayImage.h"
#include "definitions.h"
#include "byte.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

/******************* Function Implementation *******************/

BYTE** createEmptyImg(ushort rows, ushort cols)
{
	ushort i, j;
	BYTE** emptyImg;
	emptyImg = (BYTE**)malloc(sizeof(BYTE*) * rows);
	checkMemory(emptyImg);

	for (i = 0; i < rows; i++)
	{
		(emptyImg)[i] = (BYTE*)calloc(cols, sizeof(BYTE));
		checkMemory((emptyImg)[i]);
	}

	return emptyImg;
}

BOOL isAllCovered(BYTE*** img, ushort rows, ushort cols)
{
	BOOL flag = TRUE;
	ushort i = 0, j = 0;

	imgPos currPos;
	currPos[ROWS] = i;
	currPos[COLS] = j;

	while (flag && i < rows)
	{
		while (flag && j < cols)
		{
			if (isFlagSet(img, currPos) == 0)
				flag = FALSE;
			j++;
			currPos[ROWS] = i;
			currPos[COLS] = j;
		}

		i++;
		j = 0;
		currPos[ROWS] = i;
		currPos[COLS] = j;
	}

	return flag;
}

void findMinKernel(imgPos* kernel, grayImage* img, BYTE*** flag)
{
	PIXEL min = MAX_PIXEL;
	ushort i = 0, j = 0;

	imgPos currPos;
	currPos[ROWS] = i;
	currPos[COLS] = j;

	while (i < img->rows)
	{
		while (j < img->cols)
		{
			if ((img->pixels[i][j] <= min) && !isFlagSet(flag, currPos))
			{
				min = img->pixels[i][j];
				(*kernel)[ROWS] = i;
				(*kernel)[COLS] = j;
			}
			j++;
			currPos[ROWS] = i;
			currPos[COLS] = j;
		}
		i++;
		j = 0;
		currPos[ROWS] = i;
		currPos[COLS] = j;
	}
	raiseFlag(flag, *kernel);
}

grayImage* colorSegments(grayImage* img, imgPosCell** segments, uint size)
{
	imgPosCell* curr;
	uint i, color;

	grayImage* newImg = (grayImage*)malloc(sizeof(grayImage));
	checkMemory(newImg);
	newImg->cols = img->cols;
	newImg->rows = img->rows;
	newImg->pixels = createEmptyImg(newImg->rows, newImg->cols);

	for (i = 0; i < size; i++)
	{
		curr = segments[i];
		color = i * (255 / size - 1);
		while (curr)
		{
			newImg->pixels[curr->position[ROWS]][curr->position[COLS]] = color + '0';
			curr = curr->next;
		}
	}

	return newImg;
}

void freeflag(BYTE** flag, ushort rows)
{
	ushort i;
	for (i = 0; i < rows; i++)
		free(flag[i]);
	free(flag);
}

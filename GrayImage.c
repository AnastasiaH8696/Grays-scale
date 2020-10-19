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
	currPos[COLS] = j * BYTE_SIZE;

	while (flag && i < rows)
	{
		while (j < cols)
		{
			if (!isFlagSet(img, currPos));
				flag = FALSE;
			j++;
			currPos[ROWS] = i;
			currPos[COLS] = j;
		}

		i++;
		j = 0;
	}

	return flag;
}

void findMinKernel(imgPos* kernel, grayImage* img, BYTE*** flag)
{
	PIXEL min = img->pixels[(*kernel)[ROWS]][(*kernel)[COLS]];
	ushort i = 0, j = 0;

	imgPos currPos;
	currPos[ROWS] = i;
	currPos[COLS] = j;

	while (i < img->rows)
	{
		while (j < img->cols)
		{
			if ((img->pixels[i][j] < min) && !isFlagSet(flag, currPos))
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
	}
	raiseFlag(flag, *kernel);
}

void findNextMinValue(imgPos* kernel, grayImage* img)
{
	ushort i = 0, j = 0;
	while (i < img->rows)
	{
		while (j < img->cols)
		{
			if ((img->pixels[i][j] > img->pixels[(*kernel)[ROWS]][(*kernel)[COLS]]))
			{
				(*kernel)[ROWS] = i;
				(*kernel)[COLS] = j;
				return;
			}
			j++;
		}
		i++;
		j = 0;
	}
}
grayImage* colorSegments(grayImage* img, imgPosCell** segments, uint size)
{
	imgPosCell* curr;
	uint i, color;

	grayImage* newImg = (grayImage*)malloc(sizeof(grayImage));
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

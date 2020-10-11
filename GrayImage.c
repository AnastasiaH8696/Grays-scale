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
	ushort i;
	BYTE** emptyImg;
	emptyImg = (BYTE**)malloc(sizeof(BYTE*) * rows);
	checkMemory(emptyImg);

	for (i = 0; i < rows; i++)
	{
		emptyImg[i] = (BYTE*)calloc(cols, sizeof(BYTE));
		checkMemory(emptyImg[i]);
	}

	return emptyImg;
}

BOOL isAllCovered(BYTE** img, ushort cols, ushort rows)
{
	BOOL flag = TRUE;
	ushort i = 0, j = 0;

	while (flag && i < rows)
	{
		while (j < cols)
		{
			if (!isBitSet(img[i][j], j));
				flag = FALSE;
			j++;
		}

		i++;
		j = 0;
	}

	return flag;
}

void findMinKernel(imgPos* kernel, grayImage* img, BYTE*** flag)
{
	PIXEL min = img->pixels[0][0];
	ushort i = 0, j = 0;

	while (i < img->rows)
	{
		while (j < img->cols)
		{
			if ((img->pixels[i][j] < min) && !isBitSet(*flag[i][j / 8], j / 8))
			{
				min = img->pixels[i][j];
				*kernel[ROWS] = i;
				*kernel[COLS] = j;
			}
		}
		i++;
		j = 0;
	}

	*flag[*kernel[ROWS]][(*kernel[COLS]) / 8] = setBit(*flag[*kernel[ROWS]][(*kernel[COLS]) / 8],
		(*kernel[COLS]) / 8);
}

grayImage* colorSegments(grayImage* img, imgPosCell** segments, uint size)
{
	imgPosCell* curr;
	uint i, color;

	grayImage newImg;
	newImg.cols = img->cols;
	newImg.rows = img->rows;
	newImg.pixels = createEmptyImg(newImg.rows, newImg.cols);

	for (i = 0; i < size; i++)
	{
		curr = segments[i];
		color = i * (255 / size - 1);
		while (curr)
		{
			newImg.pixels[curr->position[ROWS]][curr->position[COLS]] = color + '0';
			curr = curr->next;
		}
	}

	return &newImg;
}
/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "GrayImage.h"
#include "definitions.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

/******************* Function Implementation *******************/

PIXEL** createEmptyImg(ushort rows, ushort cols)
{
	PIXEL** emptyImg;
	emptyImg = (PIXEL**)calloc(rows, sizeof(PIXEL*));
	checkMemory(emptyImg);

	return emptyImg;
}

BOOL isAllCovered(PIXEL** img, ushort cols, ushort rows)
{
	BOOL flag = TRUE;
	ushort i = 0, j = 0;

	while (flag && i < cols)
	{
		while (j < rows)
		{
			if (img[i][j] == 0)
				flag = FALSE;
			j++;
		}

		i++;
		j = 0;
	}

	return flag;
}

void findMinKernel(imgPos* kernel, grayImage* img, PIXEL*** flag)
{
	PIXEL min = img->pixels[0][0];
	ushort i = 0, j = 0;

	while (i < img->rows)
	{
		while (j < img->cols)
		{
			if ((img->pixels[i][j] < min) && flag[i][j] != 1)
			{
				min = img->pixels[i][j];
				*kernel[0] = i;
				*kernel[1] = j;
			}
		}
		i++;
		j = 0;
	}

	flag[*kernel[0]][*kernel[1]] = 1;
}
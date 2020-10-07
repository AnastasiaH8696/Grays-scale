/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "definitions.h"
#include "files.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

static ushort findPow(uchar reducedGrayLevels);
static BYTE* compress(grayImage* img, uchar reducedGrayLevels, ushort size, ushort bitSize);

/******************* Function Implementation *******************/
void saveCompressed(char* fileName, grayImage* img, uchar reducedGrayLevels)
{
	ushort pow = findPow(reducedGrayLevels);

	/*The formula: All the pixels * pow(size of each pixel) / 8(size of each byte) */
	ushort size = ((img->cols * img->rows) * pow) / BYTE_SIZE;
	BYTE* compressed; 
	compressed = compress(img, reducedGrayLevels, size, pow);

	FILE* f = fopen(fileName, "wb");
	checkFileMemory(f);

	fwrite(&(img->rows), sizeof(ushort), 1, f);
	fwrite(&(img->rows), sizeof(ushort), 1, f);
	fwrite(&reducedGrayLevels, sizeof(uchar), 1, f);
	fwrite(compressed, sizeof(BYTE), size, f);
	
	fclose(f);
}

void checkFileMemory(void* ptr)
{
	if (ptr == NULL)
	{
		puts("ERROR");
		exit(1);
		fclose(ptr);
	}
}

static ushort findPow(uchar reducedGrayLevels)
{
	ushort res = 1, base = 2, pow = 0;
	while (res != reducedGrayLevels)
	{
		res *= base;
		pow++;
	}
	return pow;
}

static BYTE* compress(grayImage* img, uchar reducedGrayLevels, ushort size, ushort bitSize)
{
	ushort i, j, freeSpace = BYTE_SIZE, byteIndex = 0;
	ushort div = (MAX_PIXEL + 1) / reducedGrayLevels; /*The right div for finding compressed value*/
	uchar currPixel;

	BYTE* compressed = (BYTE*)calloc(size, sizeof(BYTE));
	checkMemory(compressed);

	for (i = 0; i < img->rows; i++)
	{
		for (j = 0; j < img->cols; j++)
		{
			currPixel = (img->pixels[i][j]) / div;
			if (freeSpace < bitSize) /* The number between two bytes */
			{
				compressed[byteIndex] |= currPixel >> (bitSize - freeSpace);
				byteIndex++;
				compressed[byteIndex] |= currPixel << (BYTE_SIZE-bitSize) + (bitSize - freeSpace);
				freeSpace = (BYTE_SIZE - bitSize) + (bitSize - freeSpace);
			}
			else
			{
				compressed[byteIndex] |= currPixel << freeSpace - bitSize;
				freeSpace = freeSpace - bitSize;
			}
			
		}
	}

	return compressed;
}
/***** DEFINES *****/

#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "definitions.h"
#include "files.h"
#include "GrayImage.h"

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

static ushort findPow(uchar reducedGrayLevels);
static BYTE* compress(grayImage* img, uchar reducedGrayLevels, ushort size, ushort bitSize);

/******************* Function Implementation *******************/
grayImage* readPGM(char* fname)
{
	grayImage *res_grayImage = (grayImage*)malloc(sizeof(grayImage));
	checkMemory(res_grayImage);
	FILE* pgmFile;
	char version[3];
	ushort maxVal;
	ushort row, col;
	int val;
	pgmFile = fopen(fname, "rb");
	if (pgmFile == NULL) {
		fprintf(stderr, "Cannot open the file");
		exit(OPEN_FILE_ERROR);
	}
	fgets(version, sizeof(version), pgmFile);
	if (!strcmp(version, "P5")) {
		fprintf(stderr, "Wrong file version!\n");
		exit(FILE_VERSION_ERROR);
	}
	skipComments(pgmFile);
	if (fscanf(pgmFile, "%hu", &res_grayImage->cols));
	skipComments(pgmFile);
	if(fscanf(pgmFile, "%hu", &res_grayImage->rows));
	skipComments(pgmFile);
	if(fscanf(pgmFile, "%hu", &maxVal));
	fgetc(pgmFile);

	res_grayImage->pixels = createEmptyImg(res_grayImage->rows, res_grayImage->cols);
	for (row = 0; row < res_grayImage->rows; row++) {
		for (col = 0; col < res_grayImage->cols; col++) {
			if(fscanf(pgmFile, "%d", &val));
			res_grayImage->pixels[row][col] = (int)(((double)val / (double)maxVal) * 255);
		}
	}

	fclose(pgmFile);
	return res_grayImage;
}

void skipComments(FILE* fp)
{
	int ch;
	char line[100];
	while ((ch = fgetc(fp)) != EOF && isspace(ch));

	if (ch == '#') {
		fgets(line, sizeof(line), fp);
		skipComments(fp);
	}
	else {
		fseek(fp, -1, SEEK_CUR);
	}
}

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
		fprintf(stderr, "File memory failure");
		exit(FILE_MEMORY_ERROR);
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
				compressed[byteIndex] |= (currPixel) >> (bitSize - freeSpace);
				byteIndex++;
				compressed[byteIndex] |= ((currPixel) << (BYTE_SIZE-bitSize)) + (bitSize - freeSpace);
				freeSpace = (BYTE_SIZE - bitSize) + (bitSize - freeSpace);
			}
			else
			{
				compressed[byteIndex] |= (currPixel) << (freeSpace - bitSize);
				freeSpace = freeSpace - bitSize;
			}
			
		}
	}

	return compressed;
}
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
static void decompressFileIntoOther(FILE* inputFile, FILE* outputFile, ushort bitSize, ushort rows, ushort cols);
static void setFileHeader(FILE* inputFile, FILE* outputFile, ushort* rows, ushort* cols, uchar* grayLevels);
static uchar readNBits(uchar byte, ushort n, ushort startPoint);

/******************* Function Implementation *******************/
grayImage* readPGM(char* fname)
{
	grayImage *res_grayImage = (grayImage*)malloc(sizeof(grayImage));
	checkMemory(res_grayImage);
	FILE* pgmFile;
	char version[3];
	int maxVal;
	ushort row, col;
	int val;
	pgmFile = fopen(fname, "rb");
	checkFileOpening(pgmFile, fname);
	fgets(version, sizeof(version), pgmFile);
	checkPGMVersion(version);

	skipComments(pgmFile);
	if (fscanf(pgmFile, "%hu", &res_grayImage->cols));
	skipComments(pgmFile);
	if(fscanf(pgmFile, "%hu", &res_grayImage->rows));
	skipComments(pgmFile);
	if(fscanf(pgmFile, "%d", &maxVal));
	fgetc(pgmFile);

	res_grayImage->pixels = createEmptyImg(res_grayImage->rows, res_grayImage->cols);
	for (row = 0; row < res_grayImage->rows; row++) {
		for (col = 0; col < res_grayImage->cols; col++) {
			if(fscanf(pgmFile, "%d", &val));
			res_grayImage->pixels[row][col] = (char)(((double)val / (double)maxVal) * 255);
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

	FILE* compressedFile = fopen(fileName, "wb");
	checkFileOpening(compressedFile, fileName);

	fwrite(&(img->rows), sizeof(ushort), 1, compressedFile);
	fwrite(&(img->cols), sizeof(ushort), 1, compressedFile);
	fwrite(&reducedGrayLevels, sizeof(uchar), 1, compressedFile);
	fwrite(compressed, sizeof(BYTE), size, compressedFile);
	
	fclose(compressedFile);
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

static uchar readNBits(uchar byte, ushort n, ushort startPoint)
{
	uchar mask = ((uchar)1 << n) - 1;
	return ((byte >> (BYTE_SIZE - (n + startPoint))) & mask);
}

static void decompressFileIntoOther(FILE* inputFile, FILE* outputFile, ushort bitSize, ushort rows, ushort cols)
{
	ushort i = 0;
	ushort numOfBits = findPow(bitSize);
	ushort numOfReads = (rows * cols) * numOfBits / BYTE_SIZE;
	ushort printCounter = 0;
	uchar byteVal, pixelVal;
	ushort readInByte = 0; /* Indicates how many bits already read in the current byte */
	while (i < numOfReads)
	{
		if (readInByte == 0) /* If we need to read another byte */
		{
			fseek(inputFile, 5 + (i * numOfBits / BYTE_SIZE), SEEK_SET);
			fread(&byteVal, sizeof(char), 1, inputFile);
			i++;
		}
		if ((BYTE_SIZE - readInByte) >= numOfBits) /* If the whole pixel is inside of the current byte */
		{
			pixelVal = (readNBits(byteVal, numOfBits, readInByte)) * MAX_PIXEL / (bitSize-1);
			fprintf(outputFile, "%d ", pixelVal);
			printCounter++;
			readInByte = (readInByte + numOfBits) % BYTE_SIZE;
		}
		else /* The pixel stored in two separated bytes */
		{
			ushort leftToRead;
			pixelVal = (readNBits(byteVal, BYTE_SIZE - readInByte, readInByte));
			leftToRead = numOfBits - BYTE_SIZE + readInByte;
			pixelVal <<= leftToRead;
			fread(&byteVal, sizeof(char), 1, inputFile);
			i++;
			readInByte = 0;
			pixelVal |= readNBits(byteVal, leftToRead, readInByte);
			pixelVal = pixelVal * MAX_PIXEL / (bitSize - 1);
			fprintf(outputFile, "%d ", pixelVal);
			printCounter++;
			readInByte = (readInByte + leftToRead) % BYTE_SIZE;
		}
		if (printCounter % cols == 0)
			fprintf(outputFile, "\n");
	}
}

static void setFileHeader(FILE* inputFile, FILE* outputFile, ushort* rows, ushort* cols, uchar* grayLevels)
{
	fread(cols, sizeof(unsigned short), 1, inputFile);
	fread(rows, sizeof(unsigned short), 1, inputFile);
	fread(grayLevels, sizeof(unsigned char), 1, inputFile);

	fprintf(outputFile, "P2\n%hu %hu\n%hhu\n", *rows, *cols, 255);
}

void convertCompressedImageToPGM(char* compressed_file_name, char* pgm_file_name)
{
	FILE *inputFile, *outputFile;
	ushort rows, cols;
	uchar grayLevels;

	inputFile = fopen(compressed_file_name, "rb");
	checkFileOpening(inputFile, compressed_file_name);
	outputFile = fopen(pgm_file_name, "wb");
	checkFileOpening(outputFile, pgm_file_name);

	setFileHeader(inputFile, outputFile, &rows, &cols, &grayLevels);
	decompressFileIntoOther(inputFile, outputFile,grayLevels, rows, cols);
	fclose(inputFile);
	fclose(outputFile);
}
#define _CRT_SECURE_NO_WARNINGS

/***** INCLUDES *****/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "byte.h"
#include "definitions.h"
#include "files.h"
#include "GrayImage.h"
#include "ImagePositionList.h"
#include "Segment.h"
#include "tests.h"

/***** DEFINES *****/

/***** TYPEDEFS *****/

/******************* Static Function Prototypes *******************/

/******************* Function Implementation *******************/
void e2eTest()
{
	char* imgPath = "images\\mario.pgm";
	grayImage* img = readPGM(imgPath);
	imgPosCell** segments;
	int numOfSegments = findAllSegments(img, DEFAULT_THRESHOLD, &segments);
	grayImage* newImg = colorSegments(img, segments, numOfSegments);
	saveCompressed("compressed.bin", newImg, DEFAULT_REDUCED_GRAYSCALE);
	convertCompressedImageToPGM("compressed.bin", "readable_compressed.pgm");
}

void readNonExistPGMFileTest()
{
	char* nonExistPGMFilePath = "images\\nonexist.pgm";
	grayImage* img = readPGM(nonExistPGMFilePath);
}

void readWrongPGMVersionFileTest()
{
	char* nonExistPGMFilePath = "images\\p5file.pgm";
	grayImage* img = readPGM(nonExistPGMFilePath);
}
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "structs.h"
#include <stdlib.h>

#ifndef CHECK_ALLOCATION_MEMORY_FAIL
#define CHECK_ALLOCATION_MEMORY_FAIL(x) if(x == NULL) {printf("Allocation of image with pointer failed"); exit(2);}
#endif

#ifndef FORMAT_UNSUPPORTED
#define FORMAT_UNSUPPORTED printf("Format unsuported.\n"); exit(3);
#endif 

 /*
  *	 FUNCTION	Opens output file from argument FILEPATH.
  *  ARGUMENT	The argument is string FILEPATH which is the FILEPATH defined in main function by the command line
  *				arguments. It could be NULL if user didn`t specify output file.
  *  HOW		IF  FILAPATH is NULL this means the user didn`t specify path and the program asks for user 
  *				input of FILEPATH. Then if FILEPATH is incorrect a loop is called which
  *				exits only when the FILEPATH is correct and the file is opened. Return the file pointer.
  * RETURN		nothing
  */
FILE* openbmp(char* FILEPATH) {
	FILE* bmp;
	
	char temp[50];
	if (FILEPATH == NULL) {
		printf("You haven`t specified input file please enter the path to the image below.\n");
		printf("image path: ");
		scanf("%s", &temp);
		FILEPATH = &(*temp);
	}

	while ((bmp = fopen(FILEPATH, "rb")) == NULL) {
		printf("Error has occured opening the file.\nPlease enter the image path:");
		scanf("%s", &temp);
		FILEPATH = &(*temp);
	}
	return bmp;
}

/*
 *  FUNCTION	Read the header from the file passed in the argument.
 *  ARGUMENT	BITMAP_header - called by reference because data will be stored in the struct.
 *				DIB_header  - called by reference because data will be stored in the struct.
 *				FILE* is the file stream where the headers will be read.
 *  HOW			fwrite - get the information for the first header from file and write (sizeof(BITMAP_header) bytes 
 *				one time in the BITMAP_header
 *				fwrite - get the information for the first header from file and write (sizeof(DIB_header) bytes 
 *				one time in the DIB_header
 *  RETURN		nothing
 */
void readHeader(BITMAP_header* inputHeader, DIB_header* inputInfoHeader, FILE* file) {
	fread(inputHeader, sizeof(BITMAP_header), 1, file);
	fread(inputInfoHeader, sizeof(DIB_header), 1, file);
	return;
}

/*
 *  FUNCTION	Read the palette from the file.
 *	ARGUMENTS	RGB_24* -pointer to the array where the information of palette will be stored.
 *				size_t - size of the palette in the image. Used size_t instead of unsigned iny,
 *			    because according to my reasearch size_t guarantees it will be big enough to store the number
 *			    and also for portability purposes I found size_t is better than unsigned int.
 *				FILE* -  pointer to the file where the palette will be stored.
 *	HOW			fread - get the information from file, and write in RGB_24* (sizeof(RGB_24)bytes, size_t times)
 * RETURN		nothing
 */
void readpallete(RGB_24* pallete, size_t size, FILE* file) {
	fread(pallete, sizeof(RGB_24), size, file);
	return;
}

/*
 *	FUNCTION	Read the image from the file.
 *	ARGUMENTS	void** - void pointer called by address where the pixel array will be stored.
 *				char bpp - bit per pixel format
 *				width - width of the image in pixels
 *				height - height of the image in pixels
 *				FILE* - file pointer to the file.
 *	HOW			The function checks for the bpp of the image so later it can cast the void* to the appropriate
 *				struct. I calculate the width of the image in bytes and calculate the padding of the image.
 *				Then in for loop which loops every row of the image there is fread function which read the row
 *				and stores it in the pixel array. After every row the padding bytes are skipped.
 * RETURN		nothing
 */
void readimage(void** image, char bpp, unsigned int width, unsigned int height, FILE* file) {
	char padding = 0;
	switch (bpp)
	{
	case 1:
	{
		/*
		* Check if deviding by 8 is multiple by 8 otherwise I will lose 1 byte of information
		*/
		if ((width % 8) != 0) {
			width = width / 8;
			width++;
		}
		else {
			width = width / 8;
		}
		padding = (4 - ((width * sizeof(RGB_1)) % 4)) % 4;
		*image = (RGB_1*)malloc(height * width * sizeof(RGB_1));
		CHECK_ALLOCATION_MEMORY_FAIL(*image)
		for (uint32_t i = 0; i < height; i++) {
			fread(&((RGB_1*)*image)[i * width], sizeof(RGB_1), width, file);
			fseek(file, padding, SEEK_CUR);
		}
		break;
	}
	case 2:
	{
		/*
		* Check if deviding by 4 is multiple by 4 otherwise I will lose 1 byte of information
		*/
		if ((width % 4) != 0) {
			width = width / 4;
			width++;
		}
		else {
			width = width / 4;
		}
		*image = (RGB_2*)malloc(height * width * sizeof(RGB_2));
		CHECK_ALLOCATION_MEMORY_FAIL(*image)
		padding = (4 - ((width * sizeof(RGB_2)) % 4)) % 4;
		for (uint32_t i = 0; i < height; i++) {
			fread(&((RGB_2*)*image)[i * width], sizeof(RGB_2), width, file);
			fseek(file, padding, SEEK_CUR);
		}
		break;
	}
	case 4:
	{
		/*
		 * Check if deviding by 2 is multiple by 2 otherwise I will lose 1 byte of information
		 */
		if ((width % 2) != 0) {
			width = width / 2;
			width++;
		}
		else {
			width = width / 2;
		}
		padding = (4 - ((width * sizeof(RGB_4)) % 4)) % 4;
		*image = (RGB_4*)malloc(height * width * sizeof(RGB_4));
		CHECK_ALLOCATION_MEMORY_FAIL(*image)
		for (uint32_t i = 0; i < height; i++) {
			fread(&((RGB_4*)*image)[i * width], sizeof(RGB_4), width, file);
			fseek(file, padding, SEEK_CUR);
		}
		break;
	}
	case 8:
	{
		*image = (RGB_8*)malloc(height * width * sizeof(RGB_8));
		padding = (4 - ((width * sizeof(RGB_8)) % 4)) % 4;
		CHECK_ALLOCATION_MEMORY_FAIL(*image)
		for (uint32_t i = 0; i < height; i++) {
			fread(&((RGB_8*)*image)[i * width], sizeof(RGB_8), width, file);
			fseek(file, padding, SEEK_CUR);
		}
		break;
	}
	case 16:
	{
		* image = (RGB_16*)malloc(height * width * sizeof(RGB_16));
		padding = (4 - ((width * sizeof(RGB_16)) % 4)) % 4;
		CHECK_ALLOCATION_MEMORY_FAIL(*image)
		for (uint32_t i = 0; i < height; i++) {
			fread(&((RGB_16*)*image)[i * width], sizeof(RGB_16), width, file);
			fseek(file, padding, SEEK_CUR);
		}
		break;
	}
	//case 24:
	//{
	//	//printf("\n16\n");
	//	padding = (4 - ((width * sizeof(RGB_24)) % 4)) % 4;
	//	*image = (RGB_24*)malloc(height * width * sizeof(RGB_24));
	//	CHECK_ALLOCATION_MEMORY_FAIL(*image)
	//	for (uint32_t i = 0; i < height; i++) {
	//		printf("d");
	//		fread(&((RGB_24*)*image)[i * width], sizeof(RGB_24), width, file);
	//		fseek(file, padding, SEEK_CUR);
	//	}
	//	break;
	//}
	default:
		FORMAT_UNSUPPORTED
	}
}
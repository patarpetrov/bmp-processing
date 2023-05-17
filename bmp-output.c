#define _CRT_SECURE_NO_DEPRECATE
#include "structs.h"
#include <stdlib.h>

/*
 *	FUNCTION	 Opens output file from argument FILEPATH.If FILAPATH is NULL the file is opened by default path.
 *  ARGUMENT	 The argument is string FILEPATH which is the FILEPATH defined by main function. It could be none if user
 *	             didn`t specify output file.
 *  HOW			 Check if FILEPATH is NULL if it is open the file from the default filepath else open it with the filepath
 *				 passed to the function.
 *  RETURN		 file pointer to the opened file
 */
FILE* openbmpoutput(char* FILEPATH) {
	FILE* bmp;
	if (FILEPATH == NULL) {
		return bmp = fopen("images/output.bmp", "wb");
	}
	else {
		return bmp = fopen(FILEPATH, "wb");
	}
}
/*	
 *  FUNCTION	Write the header to the file passed in the argument.
 *  ARGUMENT	BITMAP_header - no need to call by reference data won`t be changed.
 *				DIB_header  - no need to call by reference data won`t be changed.
 *				FILE* is the file stream where the headers will be written.
 *  HOW			fwrite - get the information from first header and write to the file (sizeof(BITMAP_header) bytes one time)
 *				fwrite - get the information from second header and write to the file (sizeof(DIB_header) bytes one time)
 *  RETURN		nothing
 */
void outputHeader(BITMAP_header inputHeader, DIB_header inputInfoHeader, FILE* file) {
	fwrite(&inputHeader, sizeof(BITMAP_header), 1, file);
	fwrite(&inputInfoHeader, sizeof(DIB_header), 1, file);
	return;
}

/*	
 *  FUNCTION	Write the palette to the file.
 *	ARGUMENT	RGB_24* -pointer to the array where the information of palette is stored.
 *				size_t - size of the palette in the image. Used size_t instead of unsigned int because size_t,
 *			    because according to my reasearch size_t guarantees it will be big enough to store the number
 *			    and also for portability purposes I found size_t is better than unsigned int.
 *				FILE* -  pointer to the file where the palette will be stored.
 *	HOW			get the information from RGB_24*, write (sizeof(RGB_24)bytes, size_t times, write to FILE*)
 *  RETURN		nothing
 */
void outputpallete(RGB_24* pallete, size_t size, FILE* file) {
	fwrite(pallete, sizeof(RGB_24), size, file);
	return;
}

/* 
 *	FUNCTION	Write the image to the file.
 *	ARGUMENT	void** - void pointer called by address storing the pixel array.
 *				char bpp - bit per pixel format
 *				width - width of the image in pixels
 *				height - height of the image in pixels
 *				FILE* - file pointer to the file.
 *	HOW			The function checks for the bpp of the image so later it can cast the void* to the appropriate
 *				struct. I calculate the width of the image in bytes and calculate the padding of the image.
 *				Then in for loop which loops every row of the image there is fwrite function which writes the row.
 *				After every row the padding bytes are writen.
 * RETURN		nothng
 */
void outputimage(void** image, char bpp, unsigned int width, unsigned int const height, FILE* file) {
	//padding can`t be more than 4 and char is enough to store the value of it. 
	char padding;
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
		for (int i = 0; i < height; i++) {
			fwrite(&((RGB_1*)*image)[i * width], sizeof(RGB_1), width, file);
			//write the padding
			for (int k = 0; k < padding; k++)
			{
				fputc(0x00, file);
			}
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
		//caluclate the padding
		padding = (4 - ((width * sizeof(RGB_2)) % 4)) % 4;
		for (int i = 0; i < height; i++) {
			fwrite(&((RGB_2*)*image)[i * width], sizeof(RGB_2), width, file);
			//write the padding
			for (int k = 0; k < padding; k++)
			{
				fputc(0x00, file);
			}
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
		//caluclate the padding
		padding = (4 - ((width * sizeof(RGB_4)) % 4)) % 4;
		for (int i = 0; i < height; i++) {
			fwrite(&((RGB_4*)*image)[i * width], sizeof(RGB_8), width, file);
			//write the padding
			for (int k = 0; k < padding; k++)
			{
				fputc(0x00, file);
			}
		}
		break;
	}
	case 8:
	{
		//caluclate the padding
		padding = (4 - ((width * sizeof(RGB_8)) % 4)) % 4;

		for (int i = 0; i < height; i++) {
			fwrite(&((RGB_8*)*image)[i * width], sizeof(RGB_8), width, file);
			//write the padding
			for (int k = 0; k < padding; k++)
			{
				fputc(0x00, file);
			}
		}
		break;
	}
	case 16:
	{
		//caluclate the padding
		padding = (4 - ((width * sizeof(RGB_16)) % 4)) % 4;

		for (int i = 0; i < height; i++) {
			fwrite(&((RGB_16*)*image)[i * width], sizeof(RGB_16), width, file);
			//write the padding
			for (int k = 0; k < padding; k++)
			{
				fputc(0x00, file);
			}
		}
		break;
	}
	//case 24:
	//{
	//	//caluclate the padding
	//	padding = (4 - ((width * sizeof(RGB_24)) % 4)) % 4;

	//	for (int i = 0; i < height; i++) {
	//		fwrite(&((RGB_24*)*image)[i * width], sizeof(RGB_24), width, file);
	//		//write the padding
	//		for (int k = 0; k < padding; k++)
	//		{
	//			fputc(0x00, file);
	//		}
	//	}
	//	break;
	//}
	}
	return;
}
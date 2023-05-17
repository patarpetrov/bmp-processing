#define _CRT_SECURE_NO_DEPRECATE
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "structs.h"
#include "bmp-input.h"
#include "bmp-output.h"
#include "bmpcorrections.h"

#define USAGE printf("Usage: a.exe [input_filepath] [output_filepath] [instructions]\n");
#define MORE_ARGUMENTS_ERROR printf("More arguments then needed are specified.\n"); \
					USAGE \
					goto freememory;

#define ARGUMENT_NOTSUPPORTED(x) printf("Argument number %d has unsupported type.\n", (x)); USAGE goto freememory;
#define INPUT_OUTPUT_SPECIFIED printf("Input and output files already specified.\n"); USAGE exit(4);

#ifndef FORMAT_UNSUPPORTED
#define FORMAT_UNSUPPORTED printf("Format unsuported.\n"); exit(3);
#endif 

#ifndef CHECK_ALLOCATION_MEMORY_FAIL
#define CHECK_ALLOCATION_MEMORY_FAIL(x) if(x == NULL) {printf("Allocation of image with pointer failed\n"); exit(2);}
#endif

void printheader(BITMAP_header header, DIB_header inputInfoHeader);
void openbmpfile(char* FILEPATH);
void outputbpp(char* FILE_PATH);
int bmpvalid(const char* STRING);
int instructionvalid(const char* STRING);


BITMAP_header inputHeader;
DIB_header inputInfoHeader;

int height;
int width;
int SIZE_PALLETE;


void* image;
RGB_24(*pallete);

char* OUTPUT_FILE;
char* INPUT_FILE;
char* INSTRUCTIONS;
FILE* bmp;
BITMAP_header inputHeader;
DIB_header inputInfoHeader;

/*
 * FUNCTION		Removes spaces from string.
 * ARGUMENTS	char*
 * HOW			Makes a new string. Loops thought the old and if char is not ' ' saves it. At end puts 
                '/0' for end string
 * RETURN		new char* without spaces
 */
char* removeSpacesFromStr(char* string)
{
	uint32_t non_space_count = 0;

	for (int i = 0; string[i] != '\0'; i++)
	{
		if (string[i] != ' ')
		{
			string[non_space_count] = string[i];
			non_space_count++;
		}
	}
	string[non_space_count] = '\0';
	return string;
}

/*
 *	FUNCTION	Checks if user put correct file format.
 *  ARGUMENT	const char*
 *  HOW			If the last 4 char end with '.bmp' return 0.
 *  RETURN		0 if true
 */
int bmpvalid(const char* STRING) {
	return strcmp(".bmp", &STRING[strlen(STRING) - 4]);
}
/*
 * FUNCTION		Checks if instrunctions are valid.
 * ARGUMENTS	char* instructions
 * HOW			If string contains anything other then 'abcd' return -1;
 * RETURN		-1 if false; 0 if true
 */
int instructionvalid(const char* STRING) {
	for (int i = 0; i < strlen(STRING); i++) {
		if ((int)tolower(STRING[i]) < 97 || (int)tolower(STRING[i]) > 100) {
			return -1;
		}
	}
	return 0;
}
void printheader(BITMAP_header header, DIB_header inputInfoHeader) {
	printf("=============\n");
	printf("%c%c\n", header.name[0], header.name[1]);
	printf("%d\n", header.size);
	printf("Garbage\n");
	printf("%d\n", header.offset);
	printf("=============\nDIB HEADER \n");
	printf("size: %d\n", inputInfoHeader.header_size);
	printf("width: %d\n", inputInfoHeader.width);
	printf("height: %d\n", inputInfoHeader.height);
	printf("colorplanes: %d\n", inputInfoHeader.colorplanes);
	printf("bpp: %d\n", inputInfoHeader.bitsperpixel);
	printf("compression: %s\n", inputInfoHeader.compression);
	printf("image size: %d\n", inputInfoHeader.image_size);
	printf("pixels per meter x: %d\n", inputInfoHeader.PixelPerMeterX);
	printf("pixels per meter y: %d\n", inputInfoHeader.PixelPerMeterY);
	printf("Number color in pallete: %d\n", inputInfoHeader.NumberColors);
	printf("=============\n");
}

/*
 *	FUNCTION	Open bmp file and reads data that is stored only in global variables
 *  ARGUMENTS	char* FILEPATH from where to open the file
 *  HOW			filepath can be NULL but it is handled by openbmp function. The function reads the headers and 
 *              checks if the bmp picture is supported by the information in the header. Checks if there is palette in
 *              the file and if so reads it and stores it. I stored the absolute values of width and height because they
 *              can be negative. In the end call the function to read the image.
 *  RETURN		nothing
 * 
 */
void openbmpfile(char* FILEPATH) {
	bmp = openbmp(FILEPATH);

	readHeader(&inputHeader, &inputInfoHeader, bmp);
	printheader(inputHeader, inputInfoHeader);
	const unsigned char HEADERS_SIZE_COMBINED = 14 + inputInfoHeader.header_size;

	if (HEADERS_SIZE_COMBINED != 54) {
		FORMAT_UNSUPPORTED
	}

	if (inputHeader.offset > HEADERS_SIZE_COMBINED) {
		SIZE_PALLETE = (inputHeader.offset - HEADERS_SIZE_COMBINED) / 4;
		pallete = (RGB_24*)malloc(SIZE_PALLETE * sizeof(RGB_24));
		CHECK_ALLOCATION_MEMORY_FAIL(pallete);
		readpallete(pallete, SIZE_PALLETE, bmp);
		fseek(bmp, inputHeader.offset, SEEK_SET);
	}
	
	height = abs(inputInfoHeader.height);
	width = abs(inputInfoHeader.width);

	readimage(&image, inputInfoHeader.bitsperpixel, width, height, bmp);
	fclose(bmp);
}
/*
 *	FUNCTION	 Opens bmp file for output and writes all the data to it.
 *  ARGUMENT	 char* FILEPATH from where to open the file.
 *  HOW			 filepath can be NULL but it is handled by openbmpoutput function. The function writes the header.
 *				 Checks if something is stored in palette pointer and if so write it to the file. At end write the image 
 *				 pixel array to the file. Close the file.
 *  RETURN		 nothing 
 */
void outputbpp(char* FILE_PATH) {
	FILE* output = openbmpoutput(FILE_PATH);

	outputHeader(inputHeader, inputInfoHeader, output);
	if (pallete != NULL) {
		outputpallete(pallete, SIZE_PALLETE, output);
	}

	fseek(output, inputHeader.offset, SEEK_SET);
	outputimage(&image, inputInfoHeader.bitsperpixel, inputInfoHeader.width, inputInfoHeader.height, output);
	
	fclose(output);
}

/*
 *	FUNCTION	 Runs instructions
 *  ARGUMENT	 char* FILEPATH containing the instructions.
 *  HOW			 INSTRUCTIONS can be NULL so the function asks for user input when needed. After that
 *				 instructions are run and the program is closed
 *  RETURN		 nothing
 */
void runinstructions(char* INSTRUCTIONS) {
	char countCrop = 0;
	int start = 1;
	while (start) {
		if (INSTRUCTIONS == NULL) {
			char temp[50];
			//printf("You haven`t specified instructions.\n");
			printf("You can write multiple instructions to be processed.\n");
			printf("a.flip\n b.blur\n c.crop\n d.exit\n");
			printf("Your choices: ");
			scanf("%s", &temp);
			removeSpacesFromStr(temp);
			INSTRUCTIONS = &(*temp);
		}
		for (char i = 0; i < strlen(INSTRUCTIONS); i++) {
			switch (INSTRUCTIONS[i]) {
			case'a':
				flip(&image, inputInfoHeader.bitsperpixel, inputInfoHeader.width, inputInfoHeader.height);
				break;
			case'b':
				blur(&image, inputInfoHeader.bitsperpixel, inputInfoHeader.width, inputInfoHeader.height);
				break;
			case'c':
				printf("Enter crop points for crop number %d: \n", countCrop);
				int x1, y1, x2, y2;
				do {
					printf("first point\nx:");
					scanf("%d", &x1);
					printf("y: ");
					scanf("%d", &y1);
					printf("\nsecond point\nx:");
					scanf("%d", &x2);
					printf("y: ");
					scanf("%d", &y2);
					if (x1 > 0 && y1 > 0 && x2 > 0 && y2 > 0) {
						if (x1 < width && x2 < width && y1 < height && y2 < height) {
							break;
						}
					}
					else {
						break;
					}
				} while (1);
				crop(&image, &inputInfoHeader, inputInfoHeader.bitsperpixel, x1, y1, x2, y2);
				break;
			case'd':
				outputbpp(OUTPUT_FILE);
				printheader(inputHeader, inputInfoHeader);
				exit(0);
			}
		}
		INSTRUCTIONS = NULL;
	}
}

int main(int argc, char* argv[]) {
	char option;
	//Validates the arguments. the first image path in the command line arguments is always the 
	//input path and the second if it has so
	//is the output path. The instructions are also validated.
	if (argc <= 4) {
		char inputflag = 0;
		for (int i = 1; i < argc; i++) {
			if (bmpvalid(argv[i]) == 0) {
				if (inputflag == 0) {
					INPUT_FILE = &(*argv[i]);
					inputflag++;
				}
				else if(inputflag == 1) {
					OUTPUT_FILE = &(*argv[i]);
					inputflag++;
				}
				else {
					INPUT_OUTPUT_SPECIFIED
				}
			}
			else if (instructionvalid(argv[i]) == 0) {
				INSTRUCTIONS = &(*argv[i]);
			}
			else {
				ARGUMENT_NOTSUPPORTED(i)
			}
		}
	}
	else {
		MORE_ARGUMENTS_ERROR
	}
	openbmpfile(INPUT_FILE);

	runinstructions(INSTRUCTIONS);

	freememory:
	free(image);
	free(pallete);
	return 1;
}
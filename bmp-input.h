#pragma once
#include "structs.h"
#include <stdio.h>
FILE* openbmp(char* FILEPATH);
void readHeader(BITMAP_header* inputHeader, DIB_header *inputInfoHeader, FILE* file);
void readpallete(RGB_24* pallete, size_t size, FILE* file);
void readimage(void** image, char bpp, unsigned int const width, unsigned int const height, FILE* file);

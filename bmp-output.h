#pragma once
#include "structs.h"

FILE* openbmpoutput(char* FILEPATH);
void outputimage(void** image, char bpp, unsigned int width, unsigned int const height, FILE* file);
void outputHeader(BITMAP_header inputHeader, DIB_header inputInfoHeader, FILE* file);
void outputpallete(RGB_24* pallete, size_t size, FILE* file);
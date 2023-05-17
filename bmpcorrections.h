#pragma once
#include "structs.h"

void crop(void** image, DIB_header* inputInfoHeader, char bpp, int x1, int y1, int x2, int y2);
void blur(void** image, char bpp, int width, int height);
void flip(void** image, int bpp, int width, int height);
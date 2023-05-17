#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef CHECK_ALLOCATION_MEMORY_FAIL
#define CHECK_ALLOCATION_MEMORY_FAIL(x) if(x == NULL) {printf("Allocation of image with pointer failed"); exit(2);}
#endif
/*
 *	FUNCTION	Flipping the pixel array.
 *	ARGUMENTS	void** - void pointer called by address storing the pixel array.
 *				char bpp - bit per pixel format
 *				width - width of the image in pixels
 *				height - height of the image in pixels
 *	HOW			The function checks for the bpp of the image so later it can cast the void* to the appropriate
 *				struct. I calculate the size of the image and allocate space for new array. The old array is read
 *				row by row from end to start and stored in the new array. After everything the old array is 
 *				deallocated and the void pointer is set to point to the new array.
 * RETURN		nothing
 */
void flip(void** image, char bpp, int width, int height) {
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
		RGB_1* newimage = malloc(width * height * sizeof(RGB_1));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int x = height - 1; x >= 0; x--) {
			for (int y = 0; y <width; y++) {
				newimage[x * width + y] = ((RGB_1*)*image)[(height - 1 - x) * width + y];
			}
		}
		free(*image);
		*image = newimage;
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
		RGB_2* newimage = malloc(width * height * sizeof(RGB_2));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int x = height - 1; x >= 0; x--) {
			for (int y = 0; y < width; y++) {
				newimage[x * width + y] = ((RGB_2*)*image)[(height - 1 - x) * width + y];
			}
		}
		free(*image);
		*image = newimage;
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
		RGB_4* newimage = malloc(width * height * sizeof(RGB_4));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int x = height - 1; x >= 0; x--) {
			for (int y = 0; y < width; y++) {
				newimage[x * width + y] = ((RGB_4*)*image)[(height - 1 - x) * width + y];
			}
		}
		free(*image);
		*image = newimage;
		break;
	}
	case 8:
	{
		RGB_8* newimage = malloc(width * height * sizeof(RGB_8));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int x = height - 1; x >= 0; x--) {
			for (int y = 0; y < width; y++) {
				newimage[x * width + y] = ((RGB_8*)*image)[(height - 1 - x) * width + y];
			}
		}
		free(*image);
		*image = newimage;
		break;
	}
	case 16:
	{
		RGB_16* newimage = malloc(width * height * sizeof(RGB_16));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int x = height - 1; x >= 0; x--) {
			for (int y = 0; y < width; y++) {
				newimage[x * width + y] = ((RGB_16*)*image)[(height - 1 - x) * width + y];
			}
		}
		free(*image);
		*image = newimage;
		break;
	}
	}
}

/*
 *	FUNCTION	Crop the pixel array.
 *	ARGUMENTS	void** - void pointer called by address storing the pixel array.
 *				char bpp - bit per pixel format
 *				x1, y1 - cordinates of first point
 *				x2, y2 - cordinates of second point
 *				DIB_header - called by reference because after crop the image width, height in pixel changes
 *				and the function updates the header to the appopriate width, height, image_size
 *	HOW			The function checks for the bpp of the image so later it can cast the void* to the appropriate
 *				struct. I calculate the size of the cropped image and allocate space for new array. 
 *				Using arithmetic operations I was able to get only the pixels which are not cropped and store them
 *				in the new array. Old array is dealocated and the void pointer is set to point to the new array.
 * RETURN		nothing
 */
void crop(void** image, DIB_header* inputInfoHeader, char bpp, int x1, int y1, int x2, int y2) {
	switch (bpp)
	{
	case 1:
	{
		/*
		 * Check if deviding by 8 is multiple by 8 otherwise I will lose 1 byte of information
		 */
		int width_old = inputInfoHeader->width / 8;
		if ((inputInfoHeader->width % 8) != 0) {
			width_old++;
		}
		int width_new = ((x2 - x1) / 8);
		if (((x2 - x1) % 8) != 0) {
			width_new++;
		}
		RGB_1* newimage = malloc((width_new) * (y2 - y1) * sizeof(RGB_1));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		//copy to new array
		for (int y = 0; y < (y2 - y1); y++) {
			memcpy(&newimage[y * width_new], &((RGB_1*)*image)[((y1 + y) * width_old) + x1 / 8], width_new * sizeof(RGB_1));
		}
		free(*image);
		*image = newimage;
		//change the header
		inputInfoHeader->width = inputInfoHeader->width - (x1 + (inputInfoHeader->width - x2));
		inputInfoHeader->height = inputInfoHeader->height - (y1 + (inputInfoHeader->height - y2));
		inputInfoHeader->image_size = (width_new) * (y2 - y1) * sizeof(RGB_1);
		break;
	}
	case 2:
	{
		/*
		 * Check if deviding by 4 is multiple by 4 otherwise I will lose 1 byte of information
		 */
		int width_old = inputInfoHeader->width / 4;
		if ((inputInfoHeader->width % 4) != 0) {
			width_old++;
		}
		int width_new = ((x2 - x1) / 4);
		if ((x2 - x1) != 0) {
			width_new++;
		}
		RGB_2* newimage = malloc((width_new) * (y2 - y1) * sizeof(RGB_2));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int y = 0; y < (y2 - y1); y++) {
			memcpy(&newimage[y * width_new], &((RGB_2*)*image)[((y1 + y) * width_old) + x1 / 4], width_new * sizeof(RGB_2));
		}
		free(*image);
		*image = newimage;
		//change the header
		inputInfoHeader->width = inputInfoHeader->width - (x1 + (inputInfoHeader->width - x2));
		inputInfoHeader->height = inputInfoHeader->height - (y1 + (inputInfoHeader->height - y2));
		inputInfoHeader->image_size = (width_new) * (y2 - y1) * sizeof(RGB_2);
		break;
	}
	case 4:
	{
		/*
		 * Check if deviding by 2 is multiple by 2 otherwise I will lose 1 byte of information
		 */
		int width_old = inputInfoHeader->width / 2;
		if ((inputInfoHeader->width % 2) != 0) {
			width_old++;
		}
		int width_new = ((x2 - x1) / 2);
		if (((x2 - x1) % 2) != 0) {
			width_new++;
		}
		RGB_4* newimage = malloc((width_new) * (y2 - y1) * sizeof(RGB_4));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int y = 0; y < (y2 - y1); y++) {
			memcpy(&newimage[y * width_new], &((RGB_4*)*image)[((y1 + y) * width_old) + x1 / 2], width_new * sizeof(RGB_4));
		}
		free(*image);
		*image = newimage;
		//change the header
		inputInfoHeader->width = inputInfoHeader->width - (x1 + (inputInfoHeader->width - x2));
		inputInfoHeader->height = inputInfoHeader->height - (y1 + (inputInfoHeader->height - y2));
		inputInfoHeader->image_size = (width_new) * (y2 - y1) * sizeof(RGB_4);
		break;
	}
	case 8:
	{
		RGB_8* newimage = malloc((x2 - x1) * (y2 - y1) * sizeof(RGB_8));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int y = 0; y < (y2 - y1); y++) {
			memcpy(&newimage[y * (x2 - x1)], &((RGB_8*)*image)[(y1 + y) * inputInfoHeader->width + x1], (x2 - x1) * sizeof(RGB_8));
		}
		free(*image);
		*image = newimage;
		//change the header
		inputInfoHeader->width = inputInfoHeader->width - (x1 + (inputInfoHeader->width - x2));
		inputInfoHeader->height = inputInfoHeader->height - (y1 + (inputInfoHeader->height - y2));
		inputInfoHeader->image_size = (x2 - x1) * (y2 - y1) * sizeof(RGB_8);
		break;
	}
	case 16:
	{
		RGB_16* newimage = malloc((x2 - x1) * (y2 - y1) * sizeof(RGB_16));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int y = 0; y < (y2 - y1); y++) {
			memcpy(&newimage[y * (x2 - x1)], &((RGB_16*)*image)[(y1 + y) * inputInfoHeader->width + x1], (x2 - x1) * sizeof(RGB_16));
		}
		free(*image);
		*image = newimage;
		//change the header
		inputInfoHeader->width = inputInfoHeader->width - (x1 + (inputInfoHeader->width - x2));
		inputInfoHeader->height = inputInfoHeader->height - (y1 + (inputInfoHeader->height - y2));
		inputInfoHeader->image_size = (x2 - x1) * (y2 - y1) * sizeof(RGB_16);
		break;
	}
	}
}

/*
 *	FUNCTION	Blur the pixel array.
 *	ARGUMENT	void** - void pointer called by address storing the pixel array.
 *				char bpp - bit per pixel format
 *				width - width of the image in pixels
 *				height - height of the image in pixels
 *	HOW			The function checks for the bpp of the image so later it can cast the void* to the appropriate
 *				struct. I make a copy of the pixel array so when we make changes the original array stays with the 
 *				same values. Each pixel is read though loops and using two another loops I get the neighbour values
 *				of red, green and blue. If the pixel is on the border skip the non-existant neighbour pixels. The 
 *				avarage of red green and blue values are stored in the new array. Old array is dealocated and the 
 *				void pointer is set to point to the new array.
 * RETURN		nothing
 */
void blur(void** image, char bpp, int width, int height) {
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
		RGB_1* newimage = malloc(width * height * sizeof(RGB_1));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				newimage[i * width + j] = ((RGB_1*)*image)[i * width + j];
			}
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				float pixel1 = 0;
				float pixel2 = 0;
				float pixel3 = 0;
				float pixel4 = 0;
				float pixel5 = 0;
				float pixel6 = 0;
				float pixel7 = 0;
				float pixel8 = 0;
				int counter = 0;

				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						//check if on border
						if (i + k < 0 || i + k >= height)
						{
							continue;
						}
						if (j + l < 0 || j + l >= width)
						{
							continue;
						}
						//get neighbour values
						pixel1 += ((RGB_1*)*image)[(i + k) * width + (j + l)].pixel1;
						pixel2 += ((RGB_1*)*image)[(i + k) * width + (j + l)].pixel2;
						pixel3 += ((RGB_1*)*image)[(i + k) * width + (j + l)].pixel3;
						pixel4 += ((RGB_1*)*image)[(i + k) * width + (j + l)].pixel4;
						pixel5 += ((RGB_1*)*image)[(i + k) * width + (j + l)].pixel5;
						pixel6 += ((RGB_1*)*image)[(i + k) * width + (j + l)].pixel6;
						pixel7 += ((RGB_1*)*image)[(i + k) * width + (j + l)].pixel7;
						pixel8 += ((RGB_1*)*image)[(i + k) * width + (j + l)].pixel8;
						counter++;
					}
				}
				//calculate avarage and store it
				newimage[i * width + j].pixel1 = round(pixel1 / counter);
				newimage[i * width + j].pixel2 = round(pixel2 / counter);
				newimage[i * width + j].pixel3 = round(pixel3 / counter);
				newimage[i * width + j].pixel4 = round(pixel4 / counter);
				newimage[i * width + j].pixel5 = round(pixel1 / counter);
				newimage[i * width + j].pixel6 = round(pixel2 / counter);
				newimage[i * width + j].pixel7 = round(pixel3 / counter);
				newimage[i * width + j].pixel8 = round(pixel4 / counter);
			}
		}
		*image = newimage;
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
		RGB_2* newimage = malloc(width * height * sizeof(RGB_2));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				newimage[i * width + j] = ((RGB_2*)*image)[i * width + j];
			}
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				float pixel1 = 0;
				float pixel2 = 0;
				float pixel3 = 0;
				float pixel4 = 0;
				int counter = 0;

				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						//check if on border
						if (i + k < 0 || i + k >= height)
						{
							continue;
						}
						if (j + l < 0 || j + l >= width)
						{
							continue;
						}
						//get neighbour values
						pixel1 += ((RGB_2*)*image)[(i + k) * width + (j + l)].pixel1;
						pixel2 += ((RGB_2*)*image)[(i + k) * width + (j + l)].pixel2;
						pixel3 += ((RGB_2*)*image)[(i + k) * width + (j + l)].pixel3;
						pixel4 += ((RGB_2*)*image)[(i + k) * width + (j + l)].pixel4;
						counter++;
					}
				}
				//calculate avarage and store it
				newimage[i * width + j].pixel1 = round(pixel1 / counter);
				newimage[i * width + j].pixel2 = round(pixel2 / counter);
				newimage[i * width + j].pixel3 = round(pixel3 / counter);
				newimage[i * width + j].pixel4 = round(pixel4 / counter);
			}
		}
		*image = newimage;
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
		RGB_4* newimage = malloc(width * height * sizeof(RGB_4));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				newimage[i * width + j] = ((RGB_4*)*image)[i * width + j];
			}
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				float sum_red;
				float sum_blue;
				float sum_green;
				int counter;
				sum_red = sum_blue = sum_green = counter = 0;

				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						//check if on border
						if (i + k < 0 || i + k >= height)
						{
							continue;
						}
						if (j + l < 0 || j + l >= width)
						{
							continue;
						}
						//get neighbour values
						sum_red += ((RGB_4*)*image)[(i + k) * width + (j + l)].red;
						sum_blue += ((RGB_4*)*image)[(i + k) * width + (j + l)].blue;
						sum_green += ((RGB_4*)*image)[(i + k) * width + (j + l)].green;
						counter++;
					}
				}
				//calculate avarage and store it
				newimage[i * width + j].red = round(sum_red / counter);
				newimage[i * width + j].green = round(sum_green / counter);
				newimage[i * width + j].blue = round(sum_blue / counter);
			}
		}
		*image = newimage;
		break;
	}
	case 8:
	{
		RGB_8* newimage = malloc(width * height * sizeof(RGB_8));

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				newimage[i * width + j] = ((RGB_8*)*image)[i * width + j];
			}
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				float sum_red;
				float sum_blue;
				float sum_green;
				int counter;
				sum_red = sum_blue = sum_green = counter = 0;
				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						//check if on border
						if (i + k < 0 || i + k >= height)
						{
							continue;
						}
						if (j + l < 0 || j + l >= width)
						{
							continue;
						}
						//get neighbour values
						sum_red += ((RGB_8*)*image)[(i + k) * width + (j + l)].red;
						sum_blue += ((RGB_8*)*image)[(i + k) * width + (j + l)].blue;
						sum_green += ((RGB_8*)*image)[(i + k) * width + (j + l)].green;
						
						counter++;
					}
				}
				//calculate avarage and store it
				newimage[i * width + j].red = round(sum_red / counter);
				newimage[i * width + j].green = round(sum_green / counter);
				newimage[i * width + j].blue = round(sum_blue / counter);
			}
		}
		*image = newimage;
		break;
	}
	case 16:
	{

		RGB_16* newimage = malloc(width * height * sizeof(RGB_16));
		CHECK_ALLOCATION_MEMORY_FAIL(newimage)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				newimage[i * width + j] = ((RGB_16*)*image)[i * width + j];
			}
		}
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				float sum_red;
				float sum_blue;
				float sum_green;
				int counter;
				sum_red = sum_blue = sum_green = counter = 0;

				for (int k = -1; k < 2; k++)
				{
					for (int l = -1; l < 2; l++)
					{
						//check if on border
						if (i + k < 0 || i + k >= height)
						{
							continue;
						}
						if (j + l < 0 || j + l >= width)
						{
							continue;
						}
						//get neighbour values
						sum_red += ((RGB_16*)*image)[(i + k) * width + (j + l)].red;
						sum_blue += ((RGB_16*)*image)[(i + k) * width + (j + l)].blue;
						sum_green += ((RGB_16*)*image)[(i + k) * width + (j + l)].green;
						counter++;
					}
				}
				//calculate avarage and store it
				newimage[i * width + j].red = round(sum_red / counter);
				newimage[i * width + j].green = round(sum_green / counter);
				newimage[i * width + j].blue = round(sum_blue / counter);
			}
		}
		*image = newimage;
		break;
	}
	}
}
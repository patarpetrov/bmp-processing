#pragma once
#pragma pack(1)
#include <stdint.h>
#include <stdio.h>

typedef struct {
	char name[2];
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset;
}BITMAP_header;

typedef struct {
	uint32_t header_size;
	int32_t width;
	int32_t height;
	uint16_t colorplanes;
	uint16_t bitsperpixel;
	uint32_t compression;
	uint32_t image_size;
	int32_t PixelPerMeterX;
	int32_t PixelPerMeterY;
	uint32_t NumberColors;
	uint32_t NumberImportantColors;
}DIB_header;

typedef struct {
	unsigned short red;
	unsigned short green;
	unsigned short blue;
	//unsigned short garbage;
}RGB_24;

typedef struct {
	unsigned short red : 5;
	unsigned short green : 5;
	unsigned short blue : 5;
	unsigned short garbage : 1;
}RGB_16;

typedef struct {
	unsigned char red : 3;
	unsigned char green : 3;
	unsigned char blue : 2;
}RGB_8;

typedef struct {
	unsigned char red : 1;
	unsigned char green : 1;
	unsigned char blue : 1;
	unsigned char garbage : 1;
	unsigned char red1 : 1;
	unsigned char green1 : 1;
	unsigned char blue1 : 1;
	unsigned char garbage1 : 1;
}RGB_4;

typedef struct {
	unsigned char pixel1 : 2;
	unsigned char pixel2 : 2;
	unsigned char pixel3 : 2;
	unsigned char pixel4 : 2;
} RGB_2;

typedef struct {
	unsigned char pixel1 : 1;
	unsigned char pixel2 : 1;
	unsigned char pixel3 : 1;
	unsigned char pixel4 : 1;
	unsigned char pixel5 : 1;
	unsigned char pixel6 : 1;
	unsigned char pixel7 : 1;
	unsigned char pixel8 : 1;
}RGB_1;

typedef union {
	RGB_1* image_RGB1;
	RGB_2* image_RGB2;
	RGB_4* image_RGB4;
	RGB_8* image_RGB8;
	RGB_16* image_RGB16;
}union_t;
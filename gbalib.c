/**
 * Allen Wild
 * CS 1372
 * gbalib.c
 *
 * GBA Library
 */

#include <stdlib.h>
#include "gba.h"

u16* videoBuffer = VIDEO_BUFFER_0;

//I like 24 bit hex color codes (after working with
//computer graphics long enough), so here's a function
//that converts the colors for me.
u16 color_hex(int hex)
{
	int r = (hex & 0xFF0000) >> 16;
	int g = (hex & 0x00FF00) >> 8;
	int b = (hex & 0x0000FF);

	r = (r * 31) / 255;
	g = (g * 31) / 255;
	b = (b * 31) / 255;

	return (u16)(r | g<<5 | b<<10);
}

//set a single pixel to the given hex color
void setPixelh(int x, int y, int color)
{
	setPixel(x, y, color_hex(color));
}

//set a pixel to the given color
void setPixel(int x, int y, u16 color)
{
	if (x < COLS && y < ROWS)
		videoBuffer[OFFSET(x,y)] = color;
}

//draw a rectangle with hex color
void drawRecth(int x, int y, int width, int height, int color)
{
	drawRect(x, y, width, height, color_hex(color));
}

//draw a rectangle, now with DMA!
void drawRect(int x, int y, int width, int height, volatile u16 color)
{
	int i;
	
	for(i=0; i < height; i++)
	{
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(x, y+i)]);
		REG_DMA3CRT = width | DMA_SRC_FIXED | DMA_DEST_INC | DMA_ENABLE;
    }
} 
 
void fillScreenh(int color)
{
	fillScreen(color_hex(color));
}

//fill the screen a solid color with DMA
void fillScreen(volatile u16 color)
{
	REG_DMA3SAD = (u32)&color;
	REG_DMA3DAD = (u32)videoBuffer;
	REG_DMA3CRT = (160*240) | DMA_SRC_FIXED | DMA_DEST_INC | DMA_ENABLE;
}

//fill the entire screen with the given image pointer
void fillImage(const u16 *image)
{
	REG_DMA3SAD = (u32)image;
	REG_DMA3DAD = (u32)videoBuffer;
	REG_DMA3CRT = (160*240) | DMA_SRC_INC | DMA_DEST_INC | DMA_ENABLE;
}

//wait for vertical blank to draw things
void waitForVblank()
{
	while (SCANLINECOUNTER >= 160);
	while (SCANLINECOUNTER < 160);
}

//get a radom primary or secondary color
u16 randomColor()
{
	int r, g, b;
	int n = rand() % 6 + 1;
	
	r = n & 4 ? 31 : 0;
	g = n & 2 ? 31 : 0;
	b = n & 1 ? 31 : 0;
	return COLOR(r, g, b);
}

//MODE4 Things
void setPalette(const u16* pal)
{
	REG_DMA3SAD = (u32)pal;
	REG_DMA3DAD = (u32)PALETTE;
	REG_DMA3CRT = 256 | DMA_ENABLE;
}

void setPixel4(int x, int y, u8 colorIndex)
{
	int pixel = OFFSET(x, y);
	int index = pixel/2;

	if (x >= COLS || y >= ROWS)
		return;

	if (x&1) //if odd pixel, then set the first byte
		videoBuffer[index] = (videoBuffer[index] & 0x00FF) | (colorIndex << 8);
	else
		videoBuffer[index] = (videoBuffer[index] & 0xFF00) | colorIndex;
}

void fillScreen4(volatile u8 colorIndex)
{
	volatile u16 color = colorIndex<<8 | colorIndex;
	REG_DMA3SAD = (u32)&color;
	REG_DMA3DAD = (u32)videoBuffer;
	REG_DMA3CRT = (160*240/2) | DMA_SRC_FIXED | DMA_DEST_INC | DMA_ENABLE;
}

void fillImage4(const u16* image)
{
	REG_DMA3SAD = (u32)image;
	REG_DMA3DAD = (u32)videoBuffer;
	REG_DMA3CRT = (160*240/2) | DMA_SRC_INC | DMA_DEST_INC | DMA_ENABLE;
}

void drawRect4(int x, int y, int width, int height, u8 colorIndex)
{
	int i;
    volatile u16 color = colorIndex<<8 | colorIndex;
    
	for(i=0; i < height; i++)
	{
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(x, y+i)/2]);
		REG_DMA3CRT = (width/2) | DMA_SRC_FIXED | DMA_DEST_INC | DMA_ENABLE;
	}	
}

void flipPage()
{
	if (REG_DISPCTL & BUFFER1FLAG) //if it's buffer 1
	{
		REG_DISPCTL = REG_DISPCTL & ~BUFFER1FLAG;
		videoBuffer = VIDEO_BUFFER_1; //videoBuffer is the one being written to, the other buffer is displayed
	}
	else
	{
		REG_DISPCTL = REG_DISPCTL | BUFFER1FLAG;
		videoBuffer = VIDEO_BUFFER_0;
	}
}

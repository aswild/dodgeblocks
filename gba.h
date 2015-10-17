/**
 * Allen Wild
 * CS 1372
 * gba.h
 * 
 * GBA Header
 */

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;

#define REG_DISPCTL *(u16 *)0x4000000
#define MODE3 3
#define MODE4 4
#define BG2_ENABLE (1<<10)

#define ROWS 160
#define COLS 240
//#define VIDEO_BUFFER ((u16*)0x6000000)
#define VIDEO_BUFFER_0 ((u16*)0x6000000)
#define VIDEO_BUFFER_1 ((u16*)0x600A000)
#define BUFFER1FLAG (1<<4)
#define PALETTE ((u16*)0x5000000)

#define OFFSET(x,y) ((y)*COLS+(x))

#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define YELLOW COLOR(31, 31, 0)
#define CYAN COLOR(0,31,31)
#define WHITE COLOR(31,31,31)
#define BLACK 0
#define LTGREY COLOR(22, 22, 22)


//Button Codes
#define BUTTONS *(volatile unsigned int *)0x04000130
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT 	(1<<2)
#define BUTTON_START 	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)
#define KEY_DOWN(key) (~(BUTTONS) & key)

#define SCANLINECOUNTER *(volatile u16 *)0x4000006

//DMA Stuff
#define REG_DMA3SAD *(volatile u32*)0x40000D4
#define REG_DMA3DAD *(volatile u32*)0x40000D8
#define REG_DMA3CRT *(volatile u32*)0x40000DC
#define DMA_DEST_INC   (0<<21)
#define DMA_DEST_DEC   (1<<21)
#define DMA_DEST_FIXED (2<<21)
#define DMA_DEST_RESET (3<<21)
#define DMA_SRC_INC    (0<<23)
#define DMA_SRC_DEC    (1<<23)
#define DMA_SRC_FIXED  (2<<23)
#define DMA_ENABLE     (1<<31)

//Methods defined in gbalib.c
extern u16* videoBuffer;
u16 color_hex(int);
void setPixel(int, int, u16);
void drawRect(int, int, int, int, u16);
void fillScreen(u16);
void fillImage(const u16*);
void setPixelh(int, int, int);
void drawRecth(int, int, int, int, int);
void fillScreenh(int);
void waitForVblank();
u16 randomColor();

void setPalette(const u16*);
void flipPage();
void setPixel4(int, int, u8);
void fillScreen4(u8);
void fillImage4(const u16*);
void drawRect4(int, int, int, int, u8);

//text.c
extern const unsigned char fontdata[12288];
void drawChar(int, int, unsigned char, u16);
void drawString(int, int, char*, u16);
void drawChar4(int, int, unsigned char, u8);
void drawString4(int, int, char*, u8);

//dodgeblocks stuff
typedef struct block_
{
	int x;
	int y;
	int dy;
	int s;
	u8 color;
	struct block_ *next;
	struct block_ *prev;
} Block;

#define IBLACK 0
#define IWHITE 1
extern const u16 rainbowPalette[256];
extern const u16 splashBitmap[19200];
extern const u16 splashPal[256];

//blocks.c
void addBlock();
void updateBlocks();
void drawBlocks();
void removeBlock(Block*);
void clearOtherBlocks(Block*);
void clearBlocks();
Block* getRootBlock();

/**
 * Allen Wild
 * DodgeBlocks
 * CS 1372 Homework 09
 *
 * The player controls the white box with the arrow keys
 * don't hit the colored blocks, or it's game over.
 *
 * Also includes levels of increasing difficulty and a per-session high score
 *
 * Press Start to pause/resume the game
 * At game over, press start to start a new game
 */

#include "gba.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PSIZE 5
#define PY ROWS-PHEIGHT-2

enum {PLAY, PAUSE, GAMEOVER};

int update();
void draw();
void gameOver(Block*);
int hitBlock(Block*);

int x, y, dx, dy, score, state;
int frame = 0;
int level = 1;
int highscore = 0;
char message[128];
char message2[128];
char levelText[16];

int main()
{
	//init
	int i;
	Block* b;
	x = 100;
	y = 150;
	score = 0;
	message2[0] = 0;
	strncpy(levelText, "Level 1", 16);

	REG_DISPCTL = MODE4 | BG2_ENABLE;
	flipPage();

	//splash screen
	setPalette(splashPal);
	fillImage4(splashBitmap);
	flipPage();

	//wait for start
	for (i = 0; !KEY_DOWN(BUTTON_START); i++);
	while (KEY_DOWN(BUTTON_START));
	srand(i);

	//set game graphics
	setPalette(rainbowPalette);
	flipPage();

	state = PLAY;

	while (1)
	{
		switch (state)
		{
			case PLAY:
				//if should pause
				if (KEY_DOWN(BUTTON_START))
				{
					while (KEY_DOWN(BUTTON_START));
					state = PAUSE;
					break;
				}
				//update, and only draw if not game over
				if (update())
					draw();
				break;
			case PAUSE:
				//display pause message (and redraw)
				strncpy(message, "PAUSED", 127);
				draw();
				//note the lack of update() here
			case GAMEOVER: //there is intentionally no break here, PAUSE should do this too
				//wait for unpause, then go back to PLAY
				while (!KEY_DOWN(BUTTON_START));
				while (KEY_DOWN(BUTTON_START));
				state = PLAY;
				break;
		}
	}
	
	return 0;
}

//returns 1 if normal update, 0 if game over (so it doesn't redraw)
int update()
{
	Block* b;

	//process input, right or left, and up or down
	//if none of keys down, then dx = 0
	if (KEY_DOWN(BUTTON_RIGHT))
		dx = 3;
	else if (KEY_DOWN(BUTTON_LEFT))
		dx = -3;
	else
		dx = 0;
	if (KEY_DOWN(BUTTON_UP))
		dy = -3;
	else if (KEY_DOWN(BUTTON_DOWN))
		dy = 3;
	else
		dy = 0;
	//move player
	x += dx;
	y += dy;
	//if player goes off screen, put him back
	if (x < 0)
		x = 0;
	else if (x+PSIZE > COLS)
		x = COLS - PSIZE;
	if (y < 0)
		y = 0;
	else if (y+PSIZE > ROWS)
		y = ROWS - PSIZE;
	
	//increment the level every 150 frames (~5 sec at 30fps)
	if (++frame >= 150)
	{
		//only go to level 11 so it doesn't get completely impossible
		if (level < 11)
		{
			level++;
			snprintf(levelText, 16, "Level %d", level);
		}
		frame = 0;
	}
	
	//if a random number less than 20 is less than the level, then add a new block (which is randomized in addBlock())
	//for level 1, there's about a 25% chance of a new block, level 11 is >50% of a new block every frame
	if (rand() % 20 < level+1)
		addBlock();
	//move the blocks and stuff
	updateBlocks();

	b = getRootBlock();
	//don't collision check if there's not any blocks (ie at the start)
	if (b != NULL)
	{
		do
		{
			if (hitBlock(b))
			{
				//if there's a hit, game over then return gameover to the PLAY state
				gameOver(b);
				return 0;
			}
		} while ((b = b->next) != NULL);
	}

	//update message with the score
	snprintf(message, 127, "Score: %d", score);
	return 1;
}

//draws everything on the screen, fairly straightforward
void draw()
{
	fillScreen4(IBLACK);
	drawBlocks();
	drawRect4(x, y, PSIZE, PSIZE, IWHITE);
	drawString4(6, 6, message, IWHITE);
	drawString4(6, 16, message2, IWHITE);
	drawString4(186, 6, levelText, IWHITE);
	waitForVblank();
	waitForVblank();
	flipPage();
}

void gameOver(Block* b)
{
	//show only the block that killed the player
	clearOtherBlocks(b);

	//set high score and high score message
	if (score > highscore)
		highscore = score;
	if (score == highscore)
		snprintf(message2, 127, "New High Score!");
	else
		snprintf(message2, 127, "High Score: %d", highscore);
	snprintf(message, 127, "Game Over! Score: %d", score);
	//draw updated changes including lack of blocks and the updated messages
	draw();

	//actually start reseting stuff
	clearBlocks();
	score = 0;
	message2[0] = 0;
	level = 1;
	frame = 0;
	strncpy(levelText, "Level 1", 16);

	//go to GAMEOVER, which waits for start to make a new game
	state = GAMEOVER;
}

//returns true if the player hits the given block, fairly standard rectangle collision detection
int hitBlock(Block* pb)
{
	Block b = *pb;
	//return 0;
	return ((x > b.x && x < (b.x+b.s) && y > b.y && y < (b.y+b.s)) ||
			(x+PSIZE > b.x && x+PSIZE < (b.x+b.s) && y > b.y && y < (b.y+b.s)) ||
			(x > b.x && x < (b.x+b.s) && y+PSIZE > b.y && y+PSIZE < (b.y+b.s)) ||
			(x+PSIZE > b.x && x+PSIZE < (b.x+b.s) && y+PSIZE > b.y && y+PSIZE < (b.y+b.s)));
}

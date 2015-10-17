/**
 * Allen Wild
 * CS 1372 Homework 09
 * blocks.c
 * 
 * This program uses a doubly linked list (dynamically allocated with malloc())
 * to store the blocks that the player has to dodge
 * note: the struct is declared in gba.h so the main file can see it.
 */

#include <stdlib.h>
#include "gba.h"

static Block* root = NULL;
static Block* last = NULL;

extern int score;

//set the paramaters of the given block to random values
static void init(Block* b)
{
	b->s = rand() % 4 + 6;
	b->x = rand() % (COLS - b->s);
	b->y = 0;
	b->dy = rand() % 3 + 2;
	b->color = rand() % 254 + 2;
}

//add a block to the end of the list and initializes it
void addBlock()
{
	Block* n;
	//if there's nothing in the list, the set the root node
	if (root == NULL)
	{
		root = (Block*)malloc(sizeof(Block));
		init(root);
		root->prev = NULL;
		root->next = NULL;
		last = root;
	}
	else
	{
		//create the new block in memory, stored temporarily in n
		n = (Block*)malloc(sizeof(Block));
		//initialize it
		init(n);
		//there's nothing after it, and the current last is before it
		n->next = NULL;
		n->prev = last;
		//the new block is the new last
		last->next = n;
		last = n;
	}
}

//moves all of the blocks in the list and delete them if they hit the bottom of the screen
void updateBlocks()
{
	Block* b = root;
	//if there's no blocks, then GTFO
	if (root == NULL)
		return;
	do
	{
		//move it, if it goes off the edge, then remove it and increment the score
		b->y += b->dy;
		if (b->y > ROWS)
		{
			removeBlock(b);
			score++;
		}
	} while ((b = b->next) != NULL); //goto next and make sure there's soemthing there
}

//loop through all of the blocks and draw a rectabgle for them given their attributes
void drawBlocks()
{
	Block *b = root;
	if (root == NULL)
		return;
	do
	{
		drawRect4(b->x, b->y, b->s, b->s, b->color);
	} while ((b = b->next) != NULL);
}

//removes the current block and moves to the next one
void removeBlock(Block* b)
{
	Block* n = b->next;
	Block* p = b->prev;

	//special cases if the block to remove is the first or last in the list
	//the root/last have to be updated accordingly
	if (b == root)
	{
		root = n;
		root->prev = NULL;
	}
	else if (b == last)
	{
		last = p;
		last->next = NULL;
	}
	else
	{
		//normally, just link the two outside ones together
		p->next = n;
		n->prev = p;
	}
	//kill it in memory
	free(b);
}

//remove all the blocks and free their memory
void clearBlocks()
{
	Block* b = root;
	Block* n;

	//GTFO if empty
	if (root == NULL)
		return;
	do
	{
		//get the next one, then delete the current one
		n = b->next;
		free(b);
	} while ((b = n) != NULL);

	root = NULL;
	last = NULL;
}

//clears all blocks in the list except the given one
void clearOtherBlocks(Block* d)
{
	Block* b = root;
	if (root == NULL)
		return;
	do
	{
		//removeBlock() does most of the work here
		if (b != d)
			removeBlock(b);
	} while ((b = b->next) != NULL);
}

//returns the (static) root pointer so the main program can get it
Block* getRootBlock()
{
	return root;
}

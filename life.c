#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "life.h"
#include "draw.h"
#include "ui.h"


/*
   Main algorithms and data structures for Conway's Game of Life

   The algorithm and data structure for this are based on Michael Abrash's
   implementation found in the Graphics Programming Black Book. Michael's
   example is fairly archaic C++. This implementation aims to be a modern
   C implementation. 
 */


/*
	Draw all of the cells for the game at once. Mostly used to
	initialize everything

    TODO: Move all drawing routines to interface through the UI
    controller
*/
void Life_DrawAllCells(Life *game, Draw *draw)
{
	int x, y;
	for(y = 0; y < game->height; y++) {
		for(x = 0; x < game->width; x++) {
			Draw_DrawPixel(draw, x, y, Life_GetCellState(game, x, y));
		}
	}
}

/*
	What the FSM giveth, 
*/
Life *Life_MakeLife(unsigned int width, unsigned int height) 
{
	Life *game = malloc(sizeof(Life));
	assert(game != NULL);

	game->width = width;
	game->height = height;
    game->generations = 0;
	game->size = width * height;
	game->cells = malloc(sizeof(char) * game->size);
	game->temp_cells = malloc(sizeof(char) * game->size);

	assert(game->cells != NULL);
	assert(game->temp_cells != NULL);

	// Initial game state is empty
	memset(game->cells, 0, game->size);

	return game;
}

/* 
	he taketh.
*/
void Life_DestroyLife(Life *game) 
{
	assert(game != NULL);

	if(game->cells != NULL)
		free(game->cells);
	if(game->temp_cells != NULL)
		free(game->temp_cells);

	free(game);
}

/*
	Turn on a cell at a given cell location (x,y)

    This is ported from Abrash's Graphics Programming Black Book
*/
void Life_SetCell(Life *game, unsigned int x, unsigned int y) 
{
	unsigned int w = game->width, h = game->height;
	int xoleft, xoright, yoabove, yobelow;
    int idx = (y * game->width) + x;
	unsigned char *cell_ptr;

    if(idx > game->size)
        return; 

	cell_ptr = game->cells + idx;

	if (x == 0)
		xoleft = w - 1;
	else
		xoleft = -1;
	if (y == 0)
		yoabove = game->size - w;
	else
		yoabove = -w;
	if (x == (w - 1))
		xoright = -(w - 1);
	else
		xoright = 1;
	if (y == (h - 1))
		yobelow = -(game->size - w);
	else
		yobelow = w;

	*(cell_ptr) |= 0x01;
	*(cell_ptr + yoabove + xoleft) += 2;
	*(cell_ptr + yoabove) += 2;
	*(cell_ptr + yoabove + xoright) += 2;
	*(cell_ptr + xoleft) += 2;
	*(cell_ptr + xoright) += 2;
	*(cell_ptr + yobelow + xoleft) += 2;
	*(cell_ptr + yobelow) += 2;
	*(cell_ptr + yobelow + xoright) += 2;
}

/*
	Clear a cell at a given location (x,y)

    This is ported from Abrash's Graphics Programming Black Book
*/
void Life_ClearCell(Life *game, unsigned int x, unsigned int y)
{
	unsigned int w = game->width, h = game->height;
	int xoleft, xoright, yoabove, yobelow;
    int idx = (y * game->width) + x;
	unsigned char *cell_ptr;

    if(idx > game->size)
        return; 

	cell_ptr = game->cells + idx;

	// Calculate offsets -- wraps around	
	if (x == 0)
		xoleft = w - 1;
	else
		xoleft = -1;
	if (y == 0)
		yoabove = game->size - w;
	else
		yoabove = -w;
	if (x == (w - 1))
		xoright = -(w - 1);
	else
		xoright = 1;
	if (y == (h - 1))
		yobelow = -(game->size - w);
	else
		yobelow = w;

	*(cell_ptr) &= ~0x01;
	*(cell_ptr + yoabove + xoleft) -= 2;
	*(cell_ptr + yoabove ) -= 2;
	*(cell_ptr + yoabove + xoright) -= 2;
	*(cell_ptr + xoleft) -= 2;
	*(cell_ptr + xoright) -= 2;
	*(cell_ptr + yobelow + xoleft) -= 2;
	*(cell_ptr + yobelow) -= 2;
	*(cell_ptr + yobelow + xoright) -= 2;
}

/*
	Return the state of the cell at a given (x,y)
*/
int Life_GetCellState(Life *game, unsigned int x, unsigned int y)
{
	unsigned char *cell_ptr;
	cell_ptr = game->cells + (y * game->width) + x;
	return *cell_ptr & 0x01;

}

/*
	The heart of the game. Iterate the game by one generation and
	draw the results. 

    This is largely ported from Abrash's Graphics Programming Black Book
*/
void Life_Iterate(Life *game, UI *ui) //Draw *draw)
{
	unsigned int x, y, count;
	unsigned int h = game->height, w = game->width;
	unsigned char *cell_ptr, *row_cell_ptr;

	// Work from temporary map	
	memcpy(game->temp_cells, game->cells, game->size);

    game->generations++;

	// Process all cells in the current cell map
	cell_ptr = game->temp_cells;
	for (y=0; y<h; y++) {      
		x = 0;
		do {    
			// Skip 0 cells where possible
			while (*cell_ptr == 0) {
				cell_ptr++; 
				if (++x >= w) goto RowDone;
			}

			// Found a cell that requires work
			count = *cell_ptr >> 1; // # of neighboring on-cells
			if (*cell_ptr & 0x01) {
				// Cell is on; turn it off if it doesn’t have
				// 2 or 3 neighbors
				if ((count != 2) && (count != 3)) {
					Life_ClearCell(game, x, y);

                    // Couple iteration function to drawing routines for efficiency
                    if(ui != NULL)
                        UI_DrawPixel(ui, x, y, 0);
                        //Draw_DrawPixel(draw, x, y, 0);
				}
			} else {
				// Cell is off; turn it on if it has exactly 3 neighbors
				if (count == 3) {
					Life_SetCell(game, x, y);
    
                    // Couple iteration function to drawing routines for efficiency
                    if(ui != NULL)
                        UI_DrawPixel(ui, x, y, 1);
                        //Draw_DrawPixel(draw, x, y, 1);
				}
			} // move to next cell
			cell_ptr++; // advance to the next cell byte
		} while (++x < w);
RowDone:
1 + 1; // Your best bet is to look away
	}
}

/*
	Print the cells to stdout. Mostly for debugging
*/
void Life_PrintCells(Life *game) 
{
	int x, y;
	unsigned char row_cells[game->width];

	printf("[");
	for(y = 0; y < game->height; y++) {
		printf("[");
		for(x = 0; x < game->width; x++) {
			printf("%d,", Life_GetCellState(game, x, y));
		}
		printf("],\n");
	}
	printf("]");
}

void Life_ClearAllCells(Life *game) 
{
	memset(game->cells, 0, game->size);
    game->generations = 0;
}

/*
	Initialize each cell to be 1 or 0 randomly
*/
void Life_RandInit(Life *game)
{
	int x, y;
	time_t curtime = time(NULL);

	game->generations = 0;	
	srand(curtime);
	for(y = 0; y < game->height; y++) {
		for(x = 0; x < game->width; x++) {
			if(rand() % 2 == 0) {
				Life_SetCell(game, x, y);
			}
		}
	}
}

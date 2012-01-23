#ifndef _life_h
#define _life_h

#include "draw.h"

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned int size;
    unsigned long generations;
	unsigned char *cells;
	unsigned char *temp_cells;
} Life;

#include "ui.h"

void Life_DrawAllCells(Life *game, Draw *draw);

Life *Life_MakeLife(unsigned int width, unsigned int height); 

void Life_DestroyLife(Life *game); 

void Life_SetCell(Life *game, unsigned int x, unsigned int y);

void Life_ClearCell(Life *game, unsigned int x, unsigned int y);

int Life_GetCellState(Life *game, unsigned int x, unsigned int y);

void Life_Iterate(Life *game, UI *ui);

void Life_PrintCells(Life *game); 

void Life_RandInit(Life *game);

void Life_ClearAllCells(Life *game);

#endif

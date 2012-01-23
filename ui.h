#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include "draw.h"
#include "life.h"
#include "stack.h"

#ifndef _ui_h
#define _ui_h

typedef struct {
	int delay;
	int paused;
    int mouse_paused;
    int run;
    int debug;
    int lm_down;
    int draw_mode;
    float gps; // Generations per second
	Life *game;
	Draw *draw;
    Stack *zoom_stack;
    int zoom_scale;
} UI;

typedef struct {
    int x;
    int y;
} Point;

UI *UI_MakeUI(Life *game, Draw *draw);
int UI_DrawAllowed(Life *game, Draw *draw);
void UI_DestroyUI(UI *ui);
int UI_HandleEvents(UI *ui);
void UI_UpdateStatus(UI *ui);
void UI_DrawPixel(UI *ui, int x, int y, int state);
#endif



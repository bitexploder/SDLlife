#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>
#ifndef _draw_h
#define _draw_h

typedef struct { 
	SDL_Event event;
	SDL_Surface *screen;
	int pixsize;
    int screen_width;
    int screen_height;
    int status_width;
} Draw;

Draw *Draw_MakeDraw(int width, int height, int pixsize);
void Draw_DestroyDraw(Draw *draw);
SDL_Surface *Draw_InitSDL(Draw *draw, int width, int height, int pixsize);
void Draw_DrawText(Draw *draw, char *string, int size, int x, int y);
void Draw_DrawPixel(Draw *draw, unsigned int x, unsigned int y, int state);
void Draw_DrawPixelS(Draw *draw, unsigned int x, unsigned int y, int state, int pixsize);
void Draw_ClearScreen(Draw *draw);
void Draw_UpdateDisplay(Draw *draw);
#endif


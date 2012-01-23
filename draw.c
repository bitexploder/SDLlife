#include <stdlib.h>
#include <assert.h>
#include "draw.h"

Draw *Draw_MakeDraw(int width, int height, int pixsize) {
	Draw *new_draw = malloc(sizeof(Draw));
	assert(new_draw != NULL);
	
    new_draw->status_width = 196;
	new_draw->pixsize = pixsize;

	new_draw->screen = Draw_InitSDL(new_draw, width, height, pixsize);
	assert(new_draw->screen != NULL);

	return new_draw;
}

SDL_Surface *Draw_InitSDL(Draw *draw, int width, int height, int pixsize) {
	SDL_Surface *screen = NULL;

	SDL_Init( SDL_INIT_EVERYTHING );

    draw->screen_width = width * pixsize + draw->status_width;
    draw->screen_height = height * pixsize;

	screen = SDL_SetVideoMode(draw->screen_width, draw->screen_height, 32, SDL_SWSURFACE | SDL_RESIZABLE );

	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));

	return screen;
}

void Draw_DrawPixel(Draw *draw, unsigned int x, unsigned int y, int state)
{
    Draw_DrawPixelS(draw, x, y, state, draw->pixsize);
}

void Draw_DrawPixelS(Draw *draw, unsigned int x, unsigned int y, int state, int pixsize)
{
	int real_x, real_y, psize = 0;
	int x1, x2, y1, y2 = 0;

	psize = pixsize;

	real_x = x * psize;
	real_y = y * psize;

	x1 = real_x + psize;
	x2 = real_x;
	y1 = real_y;
	y2 = real_y + psize;

    //printf("Draw_DrawPixel: TR=(%d, %d), BL=(%d, %d)\n", x1, y1, x2, y2);

	if(state == 1) {
		boxRGBA(draw->screen, x1, y1, x2+1, y2-1, 200, 0, 0,  255);
	} else {
		boxRGBA(draw->screen, x1, y1, x2+1, y2-1, 255, 255, 255, 255);
	}
}

void Draw_DrawText(Draw *draw, char *string, int size, int x, int y)
{
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("freesansbold.ttf", size);

    if(!font) {
        printf("sdllife: could not load font.\n");
    }

    SDL_Color fg = {0, 0, 0};
    SDL_Color bg = {255, 255, 255}; 

    SDL_Surface *txt_surface = TTF_RenderText_Shaded(font, string, fg, bg);
    SDL_Rect txt_coord = {x, y, 0, 0};
    SDL_BlitSurface(txt_surface, NULL, draw->screen, &txt_coord);
    SDL_FreeSurface(txt_surface);
    TTF_CloseFont(font);
}

void Draw_ClearScreen(Draw *draw)
{
	Uint32 white = SDL_MapRGB(draw->screen->format, 255, 255, 255);
	SDL_FillRect(draw->screen, NULL, white);
}

void Draw_UpdateDisplay(Draw *draw) 
{
	SDL_Flip(draw->screen);
}

void Draw_DestroyDraw(Draw *draw) 
{
	assert(draw->screen != NULL);
	SDL_FreeSurface(draw->screen);
	free(draw);
	SDL_Quit();
}

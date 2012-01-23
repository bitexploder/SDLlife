#include <SDL/SDL.h>
#include <assert.h>
#include <math.h>
#include "life.h"
#include "draw.h"
#include "ui.h"

UI *UI_MakeUI(Life *game, Draw *draw)
{
    UI *ui = malloc(sizeof(UI));
    assert(ui != NULL);

    ui->paused = 0;
    ui->mouse_paused = 0;
    ui->delay = 0;
    ui->run = 0;
    ui->debug = 1;
    ui->lm_down = 0;
    ui->draw_mode = 1;
    ui->game = game;
    ui->draw = draw;
    ui->zoom_stack = NULL;
    ui->zoom_scale = 0;
    
    return ui;
}

void UI_DestroyUI(UI *ui)
{
    assert(ui != NULL);
    free(ui);
}

void UI_UpdateStatus(UI *ui)
{
    Draw *draw = ui->draw;
    Life *game = ui->game;
    int text_left_x = draw->screen_width - draw->status_width + 8;


    // TODO: Fill status area with proper bg color

    char status[512];
    snprintf(status, 512, "Generations: %ld", game->generations);
    Draw_DrawText(draw, status, 16, text_left_x, 0);

    snprintf(status, 512, "Paused: %s", (ui->paused) ? "True" : "False");
    Draw_DrawText(draw, status, 16, text_left_x, 16);

    snprintf(status, 512, "Mouse Paused: %s", (ui->mouse_paused) ? "True" : "False");
    Draw_DrawText(draw, status, 16, text_left_x, 32);

    snprintf(status, 512, "gen/sec: %f",ui->gps);
    Draw_DrawText(draw, status, 16, text_left_x, 48);
}

int UI_DrawAllowed(Life *game, Draw *draw)
{
    int width = game->width;
    int height = game->height;
    int pixsize = draw->pixsize;
    int x = draw->event.motion.x;
    int y = draw->event.motion.y;
    
    return (x < (width * pixsize) && y < (height * pixsize));
}

int UI_HandleEvents(UI *ui)
{           
    int cell_state, new_events, x, y =  0;
    Draw *draw = ui->draw;
    Life *game = ui->game;

    new_events = SDL_PollEvent(&draw->event);
    
    if(!new_events)
        return 0;

    if(draw->event.type == SDL_QUIT) {
        printf("Quitting!\n");
        ui->run = 0;
    }

    if (draw->event.type == SDL_KEYDOWN) {
        if(ui->debug) {
            printf("Name: %s\n", SDL_GetKeyName( draw->event.key.keysym.sym ) );
        }
 
        if(draw->event.key.keysym.sym == SDLK_EQUALS){
            ui->delay += 50;
        }
        if(draw->event.key.keysym.sym == SDLK_MINUS) {
            if(ui->delay >= 50) {
                ui->delay -= 50;
            }
        }
        if(draw->event.key.keysym.sym == SDLK_q) {
            ui->run = 0;
        }		

        if(draw->event.key.keysym.sym == SDLK_p) {
            ui->paused = ui->paused ^ 1; // use xor to toggle
        }

        if(draw->event.key.keysym.sym == SDLK_c) {
            Life_ClearAllCells(game);
            Draw_ClearScreen(draw);
        }		
        if(draw->event.key.keysym.sym == SDLK_r) {
            Life_ClearAllCells(game);
            Draw_ClearScreen(draw);
            Life_RandInit(game);
            Life_DrawAllCells(game, draw);
            Draw_UpdateDisplay(draw);
        }		
    }

    if(ui->lm_down) {
        if(draw->event.type == SDL_MOUSEMOTION) {
            if(ui->debug) {
                printf("SDL_MOUSEMOTION: (%d,%d)\n", draw->event.motion.x, draw->event.motion.y);
            }
            x = draw->event.motion.x / draw->pixsize;
            y = draw->event.motion.y / draw->pixsize;

            if(UI_DrawAllowed(game, draw)) {
                cell_state = Life_GetCellState(game, x, y);
                if (cell_state == 0 && ui->draw_mode == 1) {
                    Life_SetCell(game, x, y);
                    Draw_DrawPixel(draw, x, y, 1);
                } else if (cell_state == 1 && ui->draw_mode == 0) {
                    Life_ClearCell(game, x, y);
                    Draw_DrawPixel(draw, x, y, 0);
                }
                Draw_UpdateDisplay(draw);
            }
        }
    }

    if(draw->event.type == SDL_MOUSEBUTTONDOWN) {
        if(draw->event.button.button == SDL_BUTTON_WHEELUP) {
            SDL_GetMouseState(&x, &y);
            Point *point;
            Stack *sp;

            point = malloc(sizeof(Point));
            point->x = x;
            point->y = y;

            Stack_Push(&ui->zoom_stack, 0, point);
            ui->zoom_scale++;

            if(ui->debug) {
                printf("SDL_BUTTON_WHEELUP: at (%d,%d)", x, y);
    
                for(sp = ui->zoom_stack; sp != NULL; sp = sp->next) {
                    point = (Point *)sp->data;
                    printf("Zoom Stack: (%d, %d)\n", point->x, point->y);
                }
            }
        }

        if(draw->event.button.button == SDL_BUTTON_WHEELDOWN) {
            SDL_GetMouseState(&x, &y);
            Stack *sp;
            sp = ui->zoom_stack;

            if(ui->zoom_scale > 0)
                ui->zoom_scale--;

            if(sp != NULL) {
                free(sp->data);

                sp = Stack_Pop(&ui->zoom_stack); 
                free(sp);
            }

            
            if(ui->debug) {
                printf("SDL_BUTTON_WHEELDOWN: at (%d,%d)\n", x, y);
                Point *point;
                for(sp = ui->zoom_stack; sp != NULL; sp = sp->next) {
                    point = (Point *)sp->data;
                    printf("Zoom Stack: (%d, %d)\n", point->x, point->y);
                }
            }
        }

        if(draw->event.button.button == SDL_BUTTON_LEFT) {
            ui->mouse_paused = 1;
            ui->lm_down = 1;
            x = draw->event.button.x;
            y = draw->event.button.y;
            if(ui->debug) {
                printf("SDL_BUTTON_LEFT: (%d,%d)\n", x, y);
            }
            x = x / draw->pixsize;
            y = y / draw->pixsize;

            if(UI_DrawAllowed(game, draw)) {;
                if (Life_GetCellState(game, x, y) == 0) {
                    ui->draw_mode = 1; // Turn cells on until mouse button up
                    Life_SetCell(game, x, y);
                    Draw_DrawPixel(draw, x, y, 1);
                } else {
                    ui->draw_mode = 0; // Erase cells until mouse button up
                    Life_ClearCell(game, x, y);
                    Draw_DrawPixel(draw, x, y, 0);
                }
                Draw_UpdateDisplay(draw);
            }
        }
    } 

    if(draw->event.type == SDL_MOUSEBUTTONUP) {
        if(draw->event.button.button = SDL_BUTTON_LEFT) {
            ui->mouse_paused = 0;
            ui->lm_down = 0;
        }
    }

    return new_events;
}

void UI_DrawPixel(UI *ui, int x, int y, int state)
{
    Draw_DrawPixel(ui->draw, x, y, state);
}

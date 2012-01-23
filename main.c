#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <sys/time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "life.h"
#include "draw.h"
#include "ui.h"
#include "bench.h"

int main(int argc, char *argv[])
{
    int rows = 250;
    int cols = 250;
    int pixsize = 4;

    // Initialize everything
    Life *game = Life_MakeLife(cols, rows);
    Draw *draw = Draw_MakeDraw(cols, rows, pixsize);
    UI *ui = UI_MakeUI(game, draw);
    Bench *bench = Bench_MakeBench();

    // Draw initial cells
    Life_DrawAllCells(game, draw);
    Draw_UpdateDisplay(draw);

    // Setup the UI object for action
    ui->run = 1;

    // Run the main game loop
    Bench_Start(bench);
    while(ui->run) {
        if(!ui->paused && !ui->mouse_paused) {
            Life_Iterate(game, ui);
            //Life_DrawAllCells(game, draw);
            UI_UpdateStatus(ui);
            Draw_UpdateDisplay(draw);
            SDL_Delay(ui->delay);
        } else {
            UI_UpdateStatus(ui);
            Draw_UpdateDisplay(draw);
            SDL_Delay(10);
        }
        while(UI_HandleEvents(ui)) {
            // Main event handling
        }
        if(game->generations % 100 == 0) {
            Bench_End(bench);
            Bench_CalcElapsed(bench);
            ui->gps = 100.0 / (bench->elapsed / 1000.0);
            Bench_Start(bench);
        }
    }

    // Always clean up after yourself
    Life_DestroyLife(game);
    Draw_DestroyDraw(draw);
    UI_DestroyUI(ui);
}

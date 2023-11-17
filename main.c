#include <stdlib.h>
#include <time.h>
#include <SDL/SDL.h>
#include "game_of_life.h"

// Change automaton state delay
#define RESPAWN_DELAY 25

/* 
 * Function to call to launch the automaton 
 */
int 
launch_app(SDL_Surface* screen);

int 
main()
{
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "Error during SDL initialization: %s\n", SDL_GetError());
        return (EXIT_FAILURE);
    }

    int WINDOW_WIDTH = X_NB_CELLS * X_CELL_SIZE;
    int WINDOW_HEIGHT = Y_NB_CELLS * Y_CELL_SIZE;
    SDL_Surface* screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (screen == NULL) {
        fprintf(stderr, "Can't create application window: %s\n", SDL_GetError());
        SDL_Quit();
        return (EXIT_FAILURE);
    }

    int exit_code = launch_app(screen);

    SDL_FreeSurface(screen);
    SDL_Quit();
    return (exit_code);
}

/* 
 * Function to call to launch the automaton 
 */
int 
launch_app(SDL_Surface* screen)
{
    int **cels = NULL, **buffer;
    alloc_cels(&cels, &buffer, X_NB_CELLS, Y_NB_CELLS);

    SDL_Event event;
    int stop = 0;

    // The SDL timer params
    UpdateCelsTimerParam timer_param = {cels, buffer};
    SDL_AddTimer(RESPAWN_DELAY, update_cels, &timer_param);

    while (!stop) {
        SDL_PollEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                stop = 1;
                break;
        }

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        display_cels(cels, screen);
        SDL_Flip(screen);
    }

    free_cels(cels, buffer, X_NB_CELLS);
    return (EXIT_SUCCESS);
}

#include "game_of_life.h"

/*
 * Display cels on screen.
 */
void 
display_cels(int** cels, SDL_Surface* screen)
{
    int i = 0, j = 0;
    
    while (i < X_NB_CELLS) {
        j = 0;
        while (j < Y_NB_CELLS) {
            display_one_cel(cels[i][j], i, j, screen);
            ++j;
        }

        ++i;
    }
}

/*
 * Helper function used by display_cels.
 */
void 
display_one_cel(int cel, int x, int y, SDL_Surface* screen)
{
    int i = 0, j = 0,
        r = 0, g = 0, b = 0;

    if (!SIMPLE_MODE) {
        if (cel == ALIVE)
            g = 255;
        else if (cel == DEAD)
            r = 255;
        else if (cel == REBORN)
            b = 255;
        else {
            r = 255;
            g = 140;
        }
    }

    else {
        if (cel == DEAD)
            return;

        g = 255;
    }

    while (i < X_CELL_SIZE) {
        j = 0;
        while (j < Y_CELL_SIZE) {
            set_pixel(screen, x * X_CELL_SIZE + i, y * Y_CELL_SIZE + j, SDL_MapRGB(screen->format, r, g, b));
            ++j;
        }

        ++i;
    }
}

/* 
 * Update the state of the automaton.
 * That's a SDL timer callback function.
 */
Uint32 
update_cels(Uint32 interval, void* param)
{
    UpdateCelsTimerParam* param_struct = param;

    int **cels = param_struct->cels,
        **buffer = param_struct->buffer;

    int i = 0, j = 0, neighborhood;

    // Here we update the automaton state in the buffer
    while (i < X_NB_CELLS) {
        j = 0;
        
        // Apply game of life rules on each cell
        while (j < Y_NB_CELLS) {
            if (cels[i][j] == REBORN)
                buffer[i][j] = ALIVE;

            else if (cels[i][j] == DYING)
                buffer[i][j] = DEAD;

            // Number of useful cells in the neighborhood
            neighborhood = 0;
            if (i != 0) {
                update_neighborhood(cels[i - 1][j], &neighborhood);
                
                if (j != 0)
                    update_neighborhood(cels[i - 1][j - 1], &neighborhood);

                if (j != Y_NB_CELLS - 1)
                    update_neighborhood(cels[i - 1][j + 1], &neighborhood);
            }

            if (i != X_NB_CELLS - 1) {
                update_neighborhood(cels[i + 1][j], &neighborhood);
                
                if(j != 0)
                    update_neighborhood(cels[i + 1][j - 1], &neighborhood);

                if(j != Y_NB_CELLS - 1)
                    update_neighborhood(cels[i + 1][j + 1], &neighborhood);
            }

            if (j != 0)
                update_neighborhood(cels[i][j - 1], &neighborhood);

            if (j != Y_NB_CELLS - 1)
                update_neighborhood(cels[i][j + 1], &neighborhood);

            buffer[i][j] = live_cel(cels[i][j], neighborhood);
            j++;
        }

        ++i;
    }

    // Here we copy the new state in the cels matrix
    for (i = 0; i < X_NB_CELLS; ++i)
        for (j = 0; j < Y_NB_CELLS; ++j)
            cels[i][j] = buffer[i][j];

    return (interval);
}

/*
 * Update neighborhood according to game of life rules. 
 */
void 
update_neighborhood(int cel, int* neighborhood)
{
    if (cel == ALIVE || cel == REBORN)
        (*neighborhood)++;
}

/* 
 * Return the new state of cel, depending on the value of neighborhood 
 */
int 
live_cel(int cel, int neighborhood)
{
    if (!SIMPLE_MODE) {
        if (cel == ALIVE || cel == REBORN) {
            if(neighborhood == 2 || neighborhood == 3)
                return (ALIVE);

            else
                return (DYING);
        }

        else {
            if(neighborhood == 3)
                return (REBORN);

            else
                return (DEAD);
        }
    }

    else {
        if (cel == ALIVE)
            return (neighborhood == 2 || neighborhood == 3);

        else
            return (neighborhood == 3);
    }
}

/* 
 * Function used to initialize the two matrices cels & buffer with size_x * size_y element.
 */
void 
alloc_cels(int*** cels, int*** buffer, int size_x, int size_y)
{
    *cels = malloc(sizeof(int*) * size_x);
    *buffer = malloc(sizeof(int*) * size_x);

    int i = 0, j = 0;
    while (i < size_x) {
        (*cels)[i] = malloc(sizeof(int) * size_y);
        (*buffer)[i] = malloc(sizeof(int) * size_y);

        j = 0;
        while (j < size_y) {
            // The initial state is picked randomly
            (*cels)[i][j] = rand() % 2;
            ++j;
        }

        i++;
    }
}

/*
 * Free memory allocated to cels and buffer.
 */
void 
free_cels(int** cels, int** buffer, int size_x)
{
    int i = 0;
    while (i < size_x) {
        free(cels[i]);
        free(buffer[i]);
        ++i;
    }

    free(cels);
    free(buffer);
}

/* 
 * Helper function that colors one pixel. 
 */
void 
set_pixel(SDL_Surface* surf, int x, int y, Uint32 color)
{
    int bpp = surf->format->BytesPerPixel;
    Uint8* p = (Uint8*)surf->pixels + y * surf->pitch + x * bpp;

    switch (bpp) {
        case 1:
            *p = color;
            break;

        case 2:
            *(Uint16*)p = color;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (color >> 16) & 0xff;
                p[1] = (color >> 8) & 0xff;
                p[2] = color & 0xff;
            }

            else {
                p[0] = color & 0xff;
                p[1] = (color >> 8) & 0xff;
                p[2] = (color >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32*)p = color;
            break;
    }
}

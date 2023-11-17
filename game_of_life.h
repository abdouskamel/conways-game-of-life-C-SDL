#include <SDL/SDL.h>

// Number and size of cells
#define X_NB_CELLS 800
#define Y_NB_CELLS 600
#define X_CELL_SIZE 1
#define Y_CELL_SIZE 1

/*
 * When SIMPLE_MODE = 1, the program displays dead cels in black, & alive cels in green.
 * When SIMPLE_MODE = 0, the program also displays the reborn cells & cells that are going to die.
 */
#define SIMPLE_MODE 1

// This struct is passed as parameter to the SDL timer.
typedef struct
{
    int **cels, // Cels matrix
        **buffer; // This buffer is used to calculate the new state of the automaton

} UpdateCelsTimerParam;

typedef enum
{
    DEAD,
    ALIVE,
    REBORN,
    DYING

} CelsState;

/*
 * Displays cels on screen.
 */
void 
display_cels(int** cels, SDL_Surface* screen);

/*
 * Helper function used by display_cels.
 */
void 
display_one_cel(int cel, int x, int y, SDL_Surface* screen);

/* 
 * Update the state of the automaton.
 * That's an SDL timer callback function.
 */
Uint32 
update_cels(Uint32 interval, void* param);

/* 
 * Update neighborhood according to game of life rules. 
 */
void 
update_neighborhood(int cel, int* neighborhood);

/* 
 * Returns the new state of cel regarding the value of neighborhood 
 */
int 
live_cel(int cel, int neighborhood);

/* 
 * Function used to initialize the two matrices cels & buffer with size_x * size_y element. 
 */
void 
alloc_cels(int*** cels, int*** buffer, int size_x, int size_y);

/*
 * Free memory allocated to cels and buffer.
 */
void 
free_cels(int** cels, int** buffer, int size_x);

/* 
 * Helper function that colores one pixel. 
 */
void 
set_pixel(SDL_Surface* surf, int x, int y, Uint32 color);
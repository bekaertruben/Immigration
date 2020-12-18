#ifndef IMMIGRATION_H_
#define IMMIGRATION_H_

#define EMPTY 0 // an empty cell
#define A 1     // a cell of type A
#define B 2     // a cell of type B

// a single cell in a game of immigration
typedef unsigned char Cell;

// a grid of cells that form a game of immigration
typedef struct game
{
    unsigned int width;  // number of columns
    unsigned int height; // number of rows
    Cell *array_start;   // points to the start of a width x height array of cells
} *Game;

// returns a width x height game populated with empty cells
Game new_empty_game(unsigned int width, unsigned int height);

// returns a pointer to the cell at given coordinates of a game
Cell *index_game(Game game, unsigned int x, unsigned int y);

// frees the memory block holding the cells then the game itself
void deallocate_game(Game game);

// advances a game of immigration by one generation
// returns 0 on success, -1 in case of failure
int advance(Game game);

// prints a game of immigration to stdout
void print_game(Game game);

#endif
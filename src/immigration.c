#include "immigration.h"

#include <stdio.h>
#include <stdlib.h>

Game new_empty_game(unsigned int width, unsigned int height) {
    Game game = malloc(sizeof(Game));
    if (game != NULL) {
        game->width = width;
        game->height = height;
        game->array_start = calloc(width * height, sizeof(Cell));
        if (game->array_start == NULL) {
            free(game);
            return NULL;
        }
    }
    return game;
}

Cell *index_game(Game game, unsigned int x, unsigned int y) {
    if (game->array_start == NULL)
        return NULL;
    if (x >= game->width || y >= game->height) // out of bounds
        return NULL;
    return (game->array_start + y * game->width + x);
}

void deallocate_game(Game game) {
    free(game->array_start);
    free(game);
}

int advance(Game game) {
    if (game->array_start == NULL)
        return -1;
    
    // copy game bacause the original is being edited as we loop over it
    // possible optimisation: only keep a copy of the last two scanned lines
    Game old_game = new_empty_game(game->width, game->height);
    if (old_game->array_start == NULL)
        return -1;
    for (int x = 0; x < game->width; x++)
        for (int y = 0; y < game->height; y++)
            *index_game(old_game, x, y) = *index_game(game, x, y);

    for (int x = 0; x < game->width; x++) {
        for (int y = 0; y < game->height; y++) {
            int num_a = 0;
            int num_b = 0;

            for (int dx = -1; dx < 2; dx++) {
                if (x + dx < 0 || x + dx >= game->width) // don't go out of bounds
                    continue;
                
                for (int dy = -1; dy < 2; dy++) {
                    if (dx == 0 && dy == 0) // the cell itself should not be counted
                        continue;
                    if (y + dy < 0 || y + dy >= game->height)
                        continue;

                    switch (*index_game(old_game, x+dx, y+dy)) {
                    case A:
                        num_a += 1; break;
                    case B:
                        num_b += 1; break;
                    default:
                        break;
                    }
                }
            }

            int total_surrounding = num_a + num_b;
            // if cell is alive:
            if (*index_game(old_game, x, y)) {
                if (total_surrounding == 2 || total_surrounding == 3)
                    continue;
                else
                    *index_game(game, x, y) = EMPTY;
            }
            // if cell should be alive:
            else if (total_surrounding == 3) {
                if (num_a > num_b)
                    *index_game(game, x, y) = A;
                else
                    *index_game(game, x, y) = B;
            }
        }
    }

    deallocate_game(old_game);
    return 0;
}

void print_game(Game game) {
    for (int x = 0; x < game->width; x++)
        printf("---");
    printf("\n");

    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            if (*index_game(game, x, y) == EMPTY)
                printf("| |");
            else if (*index_game(game, x, y) == A)
                printf("|A|");
            else if (*index_game(game, x, y) == B)
                printf("|B|");
            else
                printf("..."); // indicates where invalid cells is located
        }
        printf("\n");
    }

    for (int x = 0; x < game->width; x++)
        printf("---");
    printf("\n");
}
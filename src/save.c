#include "save.h"

#include <stdio.h>

#define A_CHAR 'A'
#define B_CHAR 'B'
#define EMPTY_CHAR '.'

Game load_save(char* path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("file '%s' does not exist\n", path);
        return NULL;
    }

    int width, height;
    fscanf(file, "%d", &width);
    fscanf(file, "%d", &height);
    fgetc(file); // removes \n after height
    
    Game game = new_empty_game(width, height);
    if (game == NULL) {
        printf("could not allocate memory for a Game\n");
        goto close_and_return_null;
    }
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            switch (fgetc(file)) {
            case EMPTY_CHAR:
                *index_game(game, x, y) = EMPTY; break;
            case A_CHAR:
                *index_game(game, x, y) = A; break;
            case B_CHAR:
                *index_game(game, x, y) = B; break;
            case EOF:
                printf("too few characters in file '%s' to fill a board of size %d x %d\n", path, width, height);
                goto close_and_return_null;
            case '\n':
                printf("row %d in file %s is shorter than %d\n", y+1, path, width);
                goto close_and_return_null;
            default: // in case character is none of the above set it to EMPTY so that it isn't just anything
                *index_game(game, x, y) = EMPTY_CHAR; break;
            }
        }

        if (fgetc(file) != '\n' && y != height-1) {
            printf("row %d in file %s is longer than %d\n", y+1, path, width);
            goto close_and_return_null;
        }
    }

    fclose(file);
    return game;

close_and_return_null:
    deallocate_game(game);
    fclose(file);
    return NULL;
}

void save_game(Game game, char* path) {
    FILE *file = fopen(path, "w");
    fprintf(file, "%d\n", game->width);
    fprintf(file, "%d\n", game->height);

    Cell cell;
    char c;
    for (int y = 0; y < game->height; y++) {
        for (int x = 0; x < game->width; x++) {
            cell = *index_game(game, x, y);
            if (cell == A)
                c = A_CHAR;
            else if (cell == B)
                c = B_CHAR;
            else
                c = EMPTY_CHAR;
            fputc(c, file);
        }
        
        if (y != game->height-1)
            fputc('\n', file);
    }

    fclose(file);
}
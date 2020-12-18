#include "arguments.h"
#include "immigration.h"
#include "save.h"
#include "gui.h"

#include <stdio.h>

#define DEFAULT_WIDTH 30
#define DEFAULT_HEIGHT 20

#define INSTRUCTIONS "Controls:\n"\
                     " - LEFTCLICK to place a Cell in team A\n"\
                     " - RIGHTCLICK to place a Cell in team B\n"\
                     " - Pressing twice will reset to empty Cell\n"\
                     " - press SPACE to advance the game by one step\n"\
                     " - hold P to continually advance the game\n"\
                     " - press S to export the current state of the game to a savefile (use -a option to do this continually)\n"\

#define GUI_TITLE "Game of Two Lives"
#define MAX_WINDOW_WIDTH 1920
#define MAX_WINDOW_HEIGHT 1080
#define MAX_CELL_SIZE 25 // the maximum side length cells may have in the gui

#define WAITTIME 0 // a number of milliseconds to wait after every iteration
#define EXPORT_PATH "default_savefile.txt" // the file a game is exported to -> this is not asked at runtime because that would make the window "not responding"

static Game game;

const Colour BG_COLOUR = {255, 255, 255, 255}; // white
const Colour A_COLOUR = {0, 0, 0, 255};        // black
const Colour B_COLOUR = {255, 0, 0, 255};      // red

int main(int argc, char *argv[]) {
    Args args = parse_args(argc, argv);
    
    // load the game
    if (args.field != NULL) {
        game = load_save(args.field);
        if (game == NULL) {
            printf("field passed was invalid. using width and height (if passed, else uses defaults)\n");
            goto use_w_h;
        }
    } else {
        use_w_h:
        // if no width or height are passed use the defaults
        game = new_empty_game(args.width? args.width : DEFAULT_WIDTH, args.height? args.height : DEFAULT_HEIGHT);
    }

    // find the largest cell size that will still fit within maximum window dimentions
    int cell_size = MAX_CELL_SIZE;
    if (cell_size*game->width > MAX_WINDOW_WIDTH)
        cell_size = MAX_WINDOW_WIDTH/game->width;
    if (cell_size*game->height > MAX_WINDOW_HEIGHT)
        cell_size = MAX_WINDOW_HEIGHT/game->height;
    if (cell_size <= 0) {
        printf("Dimentions are too large, window will exceed maximum dimentions\n");
        cell_size = 1;
    }
    gui_init(GUI_TITLE, cell_size*game->width, cell_size*game->height);

    printf(INSTRUCTIONS);
    while (!input_states.closed) {
        read_input();
        gui_clear(BG_COLOUR);

        // handle input:
        if (!input_states.mouse_handled) {
            Cell *cell_to_change = index_game(game, mouse.x/cell_size, mouse.y/cell_size);
            if (cell_to_change != NULL) {
                if (*cell_to_change == (mouse.button? A : B))
                    *cell_to_change = EMPTY;
                else
                    *cell_to_change = mouse.button? A : B;
            }
            input_states.mouse_handled = 1;
        }
        if (input_states.play || input_states.step) {
            advance(game);
            input_states.step = 0;
        }
        if (args.autosave || input_states.save) {
            save_game(game, EXPORT_PATH);
            input_states.save = 0;
        }
        
        // draw rectangles:
        for (int x = 0; x < game->width; x++) {
            for (int y = 0; y < game->height; y++) {
                Cell cell = *index_game(game, x, y);
                if (cell == EMPTY)
                    continue;
                Colour c = (cell == A)? A_COLOUR : B_COLOUR;

                draw_rectangle(c, cell_size*x, cell_size*y, cell_size, cell_size);
            }
        }

        gui_update();
        wait(WAITTIME);
    }

    gui_quit();
    deallocate_game(game);
    /*
     * Valgrind still shows a memory leak, this is likely due to SDL
     * see:
     *  - https://stackoverflow.com/a/1997280/9387553
     *  - https://discourse.libsdl.org/t/why-sdl-is-reported-memory-leak-by-valgrind/15324/2
     * 
     * The amount of memory lost seems to be constant. It doesn't increase with the number of iterations
     * so it's probably nothing to worry about
     */
    return 0;
}
#include "arguments.h"
#include <stdlib.h>
#include <stdio.h>

#define HELP_MSG "usage: %s [options]\n"\
                 "options and arguments:\n"\
                 "   -f field            : file to read starting field from\n"\
                 "   -w width -h height  : dimentions for new starting field (ignored if field passed)\n"\
                 "   -a                  : autosave - if passed the game will be saved every iteration\n"\
                 "if the same option is passed multiple times, only the last is counted\n"
#define UNKNOWN_OPTION "Unknown option: %s\n"
#define INVALID_ARGUMENT "Invalid argument '%s' for option %s (expected %s)\n"
#define UNEXPECTED_ARGUMENT "Did not expect argument '%s'\n"

Args parse_args(int argc, char *argv[]) {
    Args args = {
        width: 0,
        height: 0,
        field: NULL,
        autosave: 0
    };

    char* command_name = argv[0];
    while (*++argv != NULL)
    {
        if (**argv != '-') { // this is not an option
            printf(UNEXPECTED_ARGUMENT, *argv);
            goto show_help_and_exit;
        }

        char* opt = *argv; // points to the option (f, w, h or a) we are currently resolving
        // the for loop allows chaining of options eg: ./game -wha 30 20
        // each of the options increments argv and resolves the string to the requisite argument
        for (int i = 1; opt[i] != '\0'; i++) {
            switch (*(opt+i)){
            case 'f':
                args.field = *++argv;
                if (args.field == NULL) {
                    printf(INVALID_ARGUMENT, *argv, opt, "path");
                    goto show_help_and_exit;
                }
                break;
            
            case 'w':
                args.width = atoi(*++argv);
                if (args.width == 0) {
                    printf(INVALID_ARGUMENT, *argv, opt, "integer strictly greater than 0");
                    goto show_help_and_exit;
                }
                break;
            
            case 'h':
                args.height = atoi(*++argv);
                if (args.height == 0) {
                    printf(INVALID_ARGUMENT, *argv, opt, "integer strictly greater than 0");
                    goto show_help_and_exit;
                }
                break;
            
            case 'a':
                args.autosave = 1;
                break;
            
            default:
                printf(UNKNOWN_OPTION, opt);
                goto show_help_and_exit;
            }
        }
    }

    return args;

show_help_and_exit:
    printf(HELP_MSG, command_name);
    exit(-1);
}
#ifndef ARGS_H_
#define ARGS_H_

typedef struct args
{
    unsigned int width; // passed with -w
    unsigned int height; // passed with -h
    char* field; // passed with -f
    int autosave; // set to 1 if -a is passed
} Args;

// resolves the CL options passed to an args struct
Args parse_args(int argc, char *argv[]);

#endif
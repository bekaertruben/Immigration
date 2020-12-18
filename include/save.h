#ifndef SAVE_H_
#define SAVE_H_

#include "immigration.h"

// loads a game from a file
Game load_save(char* path);

// saves a game to a file
void save_game(Game game, char* path);

#endif

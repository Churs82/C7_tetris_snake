#ifndef GAME_H
#define GAME_H

#ifdef SNAKE
#include "libsnake.h"
#else
#include "libtetris.h"
#endif
#include "interface.h"

void game_loop();

#endif
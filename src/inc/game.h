#ifndef GAME_H
#define GAME_H

#include "defines.h"
#include "interface.h"
#ifdef SNAKE
#include "libsnake.h"
#else
#include "libtetris.h"
#endif

void game_loop();

#endif
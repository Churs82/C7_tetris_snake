#include <sys/time.h>
#include <time.h>

#include "fsm.h"
#include "lib.h"

void userInput(UserAction_t action, bool hold) { _userAction(action); }

GameInfo_t updateCurrentState() { return _updateCurrentState(); }

game_state getState() { return _getState(); }
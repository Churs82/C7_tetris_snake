#include "libsnake.h"

#include <sys/time.h>
#include <time.h>

#include "fsm.h"

void userInput(UserAction_t action, bool hold) {
  if (!updateCurrentState().pause || action == Pause || action == Terminate)
    _userAction(action);
}

GameInfo_t updateCurrentState() { return _updateCurrentState(); }

game_state getState() { return _getState(); }
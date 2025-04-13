#include <sys/time.h>
#include <time.h>

#include "lib.h"
#include "model.h"

using s21::snake::model;

void userInput(UserAction_t action, bool hold) {
  if (!updateCurrentState().pause || action == Pause || action == Terminate)
    userAction(action);
}

GameInfo_t updateCurrentState() { return updateState(); }

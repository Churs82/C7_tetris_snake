extern "C" {
#include "lib.h"
}

#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

void userInput(UserAction_t action, bool hold) {
  if (!updateCurrentState().pause || action == Pause || action == Terminate)
    s21::snake::model::instance::get()->userAction(action);
};

GameInfo_t updateCurrentState() {
  return s21::snake::model::instance::get()->updateState();
};

#ifdef __cplusplus
}
#endif
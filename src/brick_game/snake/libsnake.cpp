#include <sys/time.h>
#include <time.h>

#include "lib.h"
#include "model.h"

using s21::snake::model;

static model* getModel() {
  static std::unique_ptr<model> instance = s21::snake::model::factory::create();
  return instance.get();
}

void userInput(UserAction_t action, bool hold) {
  if (!updateCurrentState().pause || action == Pause || action == Terminate)
    getModel()->userAction(action);
}

GameInfo_t updateCurrentState() { return getModel()->updateState(); }

#include "lib.h"
#include "model.h"

void userInput(UserAction_t action, bool hold) {
  s21::snake::model::instance::get()->userAction(action);
};

GameInfo_t updateCurrentState() {
  return s21::snake::model::instance::get()->updateState();
};

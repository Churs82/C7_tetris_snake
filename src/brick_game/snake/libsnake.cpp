#include "lib.h"
#include "model.h"
#include "states.h"

void userInput(UserAction_t action, bool hold) {
  s21::snake::Model::Instance::Get()->UserAction(action);
};

GameInfo_t updateCurrentState() {
  return s21::snake::Model::Instance::Get()->UpdateState();
};

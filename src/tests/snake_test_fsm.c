#include "snake_test.h"
START_TEST(snake_test_init_1) {
  GameInfo_t gs = updateCurrentState();
  userInput(Terminate, false);
  gs = updateCurrentState();
  ck_assert_int_eq(gs.speed, 0);
}
END_TEST

START_TEST(snake_test_gameover) {
  userInput(Start, false);
  updateCurrentState();
  userInput(Start, false);
  for (int i = 0; i < 1000; i++) {
    userInput(Down, false);
    updateCurrentState();
  }
  ck_assert_int_eq(getState(), GAME_OVER);
  userInput(Start, false);
  userInput(Terminate, false);
  userInput(Terminate, false);
}
END_TEST

START_TEST(snake_test_move) {
  userInput(Start, false);
  updateCurrentState();
  userInput(Start, false);
  updateCurrentState();
  for (int i = 0; i < 5; i++) userInput(Left, false);
  userInput(Down, false);
  for (int i = 0; i < 10; i++) userInput(Right, false);
  userInput(Action, false);
  updateCurrentState();
  userInput(Terminate, false);
  userInput(Terminate, false);
  ck_assert_int_eq(getState(), EXIT_STATE);
}
END_TEST

START_TEST(snake_test_pause) {
  updateCurrentState();
  userInput(Start, false);
  updateCurrentState();
  userInput(Start, false);
  userInput(Pause, false);
  ck_assert_int_eq(updateCurrentState().pause, 1);
  userInput(Left, false);
  userInput(Terminate, false);
  userInput(Pause, false);
  userInput(Terminate, false);
  userInput(Terminate, false);
}
END_TEST

Suite *snake_test_fsm() {
  Suite *s = suite_create("\033[33m-=FSM tests=-\033[0m");
  TCase *tc = tcase_create("fsm");
  tcase_add_test(tc, snake_test_init_1);
  tcase_add_test(tc, snake_test_gameover);
  tcase_add_test(tc, snake_test_move);
  tcase_add_test(tc, snake_test_pause);
  suite_add_tcase(s, tc);
  return s;
}

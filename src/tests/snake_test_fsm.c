#include "snake_test.h"
START_TEST(snake_test_init_1) {
  GameInfo_t gs = updateCurrentState();
  userInput(Terminate, false);
  gs = updateCurrentState();
  ck_assert_int_eq(gs.speed, 0);
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
  tcase_add_test(tc, snake_test_pause);
  suite_add_tcase(s, tc);
  return s;
}

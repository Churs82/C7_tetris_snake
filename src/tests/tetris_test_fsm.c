#include "tetris_test.h"
START_TEST(tetris_test_init_1) {
  userInput(Start, false);
  updateCurrentState();
  userInput(Terminate, false);
  GameInfo_t gs = updateCurrentState();
  ck_assert_int_eq(gs.speed, 0);
}
END_TEST

START_TEST(tetris_test_gameover) {
  updateCurrentState();
  userInput(Start, false);
  updateCurrentState();
  ck_assert_ptr_ne(updateCurrentState().field, NULL);

  for (int i = 0; i < 1000; i++) {
    userInput(Down, false);
    updateCurrentState();
  }
  userInput(Terminate, false);
  ck_assert_ptr_eq(updateCurrentState().field, NULL);
}
END_TEST

START_TEST(tetris_test_move) {
  userInput(Start, false);
  updateCurrentState();

  for (int i = 0; i < 5; i++) userInput(Left, false);
  userInput(Down, false);
  for (int i = 0; i < 10; i++) userInput(Right, false);
  userInput(Action, false);
  userInput(Terminate, false);
  userInput(Terminate, false);
  ck_assert_ptr_eq(updateCurrentState().field, NULL);
}
END_TEST

START_TEST(tetris_test_pause) {
  updateCurrentState();
  userInput(Start, false);
  updateCurrentState();
  userInput(Start, false);
  userInput(Pause, false);
  ck_assert_int_eq(updateCurrentState().pause, 1);
  userInput(Left, false);
  ck_assert_int_eq(updateCurrentState().pause, 0);
  userInput(Terminate, false);
  userInput(Pause, false);
  userInput(Terminate, false);
  userInput(Terminate, false);
}
END_TEST

Suite *tetris_test_fsm() {
  Suite *s = suite_create("\033[33m-=Tetris FSM tests=-\033[0m");
  TCase *tc = tcase_create("fsm");
  tcase_add_test(tc, tetris_test_init_1);
  tcase_add_test(tc, tetris_test_gameover);
  tcase_add_test(tc, tetris_test_move);
  tcase_add_test(tc, tetris_test_pause);
  suite_add_tcase(s, tc);
  return s;
}

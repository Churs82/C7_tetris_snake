#ifndef TETRIS_TEST_H
#define TETRIS_TEST_H

#include <check.h>
#include <stdio.h>

#include "lib.h"

#define TESTS tetris_test_fsm(),

#define COPY_FIELD(field, newfield)      \
  for (int i = 0; i < ROWS_MAP; i++) {   \
    for (int j = 0; j < COLS_MAP; j++) { \
      newfield[i][j] = field[i][j];      \
    }                                    \
  }
#define COMPARE_FIELD(field, newfield, changed) \
  for (int i = 0; i < ROWS_MAP; i++) {          \
    for (int j = 0; j < COLS_MAP; j++) {        \
      if (field[i][j] != newfield[i][j]) {      \
        changed = true;                         \
        i = ROWS_MAP;                           \
        j = COLS_MAP;                           \
      }                                         \
    }                                           \
  }

Suite *tetris_test_fsm();

#endif  // TETRIS_TEST_H

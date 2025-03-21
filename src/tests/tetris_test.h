#ifndef TETRIS_TEST_H
#define TETRIS_TEST_H

#include <check.h>
#include <stdio.h>

#include "libtetris.h"

#define TESTS tetris_test_fsm(),

Suite *tetris_test_fsm();

#endif  // TETRIS_TEST_H

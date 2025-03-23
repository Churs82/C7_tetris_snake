#include "snake_test.h"

void printBoardTest(GameInfo_t game_info) {
  for (int i = ROWS_MAP - 1; i >= 0; i--) {
    for (int j = 0; j < COLS_MAP; j++) {
      if (game_info.field[i][j] == 0) {
        printf(" *");
      } else {
        printf(" %d", game_info.field[i][j]);
      }
    }
    printf("\n");
  }
}

int main(void) {
  int failed = 0;
  int total = 0;

  Suite *snake_test[] = {TESTS NULL};

  for (int i = 0; snake_test[i]; i++) {
    SRunner *sr = srunner_create(snake_test[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    total += srunner_ntests_run(sr);
    srunner_free(sr);
  }
  printf("\033[41m========= FAILED : %-3d =========\033[0m\n", failed);
  printf("\033[42m\033[30m========= SUCCESS: %-3d =========\033[0m\n",
         total - failed);
  printf("\033[42m\033[30m========= TOTAL: %-3d =========\033[0m\n", total);

  return failed == 0 ? 0 : 1;
}

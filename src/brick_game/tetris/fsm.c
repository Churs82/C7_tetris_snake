#include "fsm.h"

#include <sys/time.h>
#include <time.h>

#include "libtetris.h"

static GameInfo_t game_info = {0};
static game_state state;

static void checkTime();
static void start();
static void spawn();
static void moveright();
static void moveleft();
static void rotate();
static void down();
static void attach();
static void anihilate();
static void masshift(int start);
static short checkBounds(short dir);
static void down_sw();
static void spawn_sw();
static void attach_sw();
static void gameover_sw();
static void move_sw();
static void exitstate_sw();
static void pausetoggle();
static void doexit();
static void restart();
static void scoreAdd(int scoreadd);
static void updateHighScore();
static void findBounds(int *left, int *top, int *right, int *bottom);

static void pausetoggle() { game_info.pause = !game_info.pause; }

static void spawn_sw() { state = SPAWN; }

static void down_sw() { state = DOWN_SHIFTING; }

static void attach_sw() { state = ATTACHING; }

static void gameover_sw() { state = GAME_OVER; }

static void move_sw() { state = MOVING; }

static void exitstate_sw() { state = EXIT_STATE; }

void _userAction(UserAction_t action) {
  if (game_info.pause) checkTime();
  if (!game_info.pause || action == Pause)
    if (FSM_TABLE[state][action]) FSM_TABLE[state][action]();
}

GameInfo_t _updateCurrentState() {
  if (game_info.speed == 0 && state != EXIT_STATE) start();
  if (!game_info.pause && FSM_TRANSFER[state]) FSM_TRANSFER[state]();
  return game_info;
}

game_state _getState() { return state; }

static void checkTime() {
  static struct timeval last_time;
  struct timeval current_time;
  gettimeofday(&current_time, NULL);

  if (!last_time.tv_sec || game_info.pause ||
      last_time.tv_sec > current_time.tv_sec)
    last_time = current_time;
  else if (((current_time.tv_sec * CLOCKS_PER_SEC + current_time.tv_usec) -
            (last_time.tv_sec * CLOCKS_PER_SEC + last_time.tv_usec)) >=
           CLOCKS_PER_SEC * 2 / game_info.speed) {
    down_sw();
    last_time = current_time;
  }
}

static void start() {
  game_info.speed = 1;
  game_info.score = 0;
  game_info.pause = 0;
  game_info.level = 1;
  srand(time(NULL));
  if (!game_info.next) {
    game_info.next = (int **)calloc(FIGURE_H, sizeof(int *));
    for (int i = 0; i < FIGURE_H; i++) {
      game_info.next[i] = (int *)calloc(FIGURE_W, sizeof(int));
    }
  }
  if (!game_info.field) {
    game_info.field = (int **)calloc(ROWS_MAP + FIGURE_H, sizeof(int *));
    for (int i = 0; i < ROWS_MAP + FIGURE_H; i++)
      game_info.field[i] = (int *)calloc(COLS_MAP, sizeof(int));
  }
  state = START;

  FILE *hsfile = fopen(HS_FILE, "r");
  if (hsfile) {
    fscanf(hsfile, "%d", &game_info.high_score);
    fclose(hsfile);
  }
}

static void spawn() {
  int bibidibabidiboo = rand() % FIGURES_COUNT * FIGURE_H;
  /* start row of figure is max(highest) row of field */
  short start_h = ROWS_MAP + FIGURE_H - 1;
  /* start column of figure is middle of field minus half width of figure */
  short start_w = COLS_MAP / 2 - FIGURE_W / 2;
  short emptyFlag = 0;
  for (short i = start_h; i > start_h - FIGURE_H; i--)
    for (short j = start_w; j < start_w + FIGURE_W; j++)
      emptyFlag += game_info.field[i][j] =
          game_info.next[start_h - i][j - start_w];
  for (short i = 0; i < FIGURE_H; i++)
    for (short j = 0; j < FIGURE_W; j++)
      game_info.next[i][j] = FIGURES[bibidibabidiboo + i][j];
  if (!emptyFlag)
    spawn();
  else
    (down_sw());
}

static void moveright() {
  if (checkBounds(-1))
    for (short i = 0; i < ROWS_MAP + FIGURE_H; i++)
      for (short j = COLS_MAP - 1; j >= 0; j--)
        if (game_info.field[i][j] > 1) {
          game_info.field[i][j + 1] = game_info.field[i][j];
          game_info.field[i][j] = 0;
        }
}

static void moveleft() {
  if (checkBounds(1))
    for (short i = 0; i < ROWS_MAP + FIGURE_H; i++)
      for (short j = 0; j < COLS_MAP; j++)
        if (game_info.field[i][j] > 1) {
          game_info.field[i][j - 1] = game_info.field[i][j];
          game_info.field[i][j] = 0;
        }
}

static void rotate() {
  int right = 0, top = 0, bottom = ROWS_MAP + FIGURE_H, left = COLS_MAP;
  findBounds(&left, &top, &right, &bottom);
  short possibility = 1;
  int color = 0;
  for (short i = bottom; i <= top && possibility; i++)
    for (short j = left; j <= right && possibility; j++)
      if (game_info.field[i][j] > 1) {
        if (!color) color = game_info.field[i][j];
        int new_row = top - (j - left);
        int new_col = left + (i - bottom);
        if (new_col < 0 || new_row < 0 || new_col >= COLS_MAP ||
            new_row >= ROWS_MAP + FIGURE_H ||
            game_info.field[new_row][new_col] == 1)
          possibility = 0;
        else if (game_info.field[new_row][new_col] == 0)
          game_info.field[new_row][new_col] = -color;
        else if (game_info.field[new_row][new_col] == color)
          game_info.field[new_row][new_col]++;
      }
  for (short i = 0; i < ROWS_MAP + FIGURE_H; i++)
    for (short j = 0; j < COLS_MAP; j++)
      if (game_info.field[i][j] < 0)
        game_info.field[i][j] = possibility * color;
      else if (game_info.field[i][j] > color)
        game_info.field[i][j] = color;
      else if (game_info.field[i][j] == color)
        game_info.field[i][j] = possibility ? 0 : color;
}

static void findBounds(int *left, int *top, int *right, int *bottom) {
  for (short j = 0; j < COLS_MAP; j++)
    for (short i = 0; i < ROWS_MAP + FIGURE_H; i++)
      if (game_info.field[i][j] > 1) {
        if (*right <= j) *right = j;
        if (*top <= i) *top = i;
        if (*left >= j) *left = j;
        if (*bottom >= i) *bottom = i;
      }
  while (*left + *top - *bottom >= COLS_MAP) *left = *left - 1;
}

static void down() {
  if (!checkBounds(0))
    attach_sw();
  else {
    spawn_sw();
    for (short j = 0; j < COLS_MAP; j++) {
      for (short i = 0; i < ROWS_MAP + FIGURE_H; i++) {
        if (game_info.field[i][j] > 1) {
          game_info.field[i - 1][j] = game_info.field[i][j];
          game_info.field[i][j] = 0;
          move_sw();
        }
      }
    }
  }
}

static void attach() {
  for (short i = 0; i < ROWS_MAP + FIGURE_H; i++)
    for (short j = 0; j < COLS_MAP; j++)
      if (game_info.field[i][j] > 1) game_info.field[i][j] = 1;
  anihilate();
  spawn_sw();
  for (short j = 0; j < COLS_MAP; j++)
    if (game_info.field[ROWS_MAP - 1][j] == 1) gameover_sw();
}
/**
 * Removes full lines from the game field, updates the score and high score.
 *
 * This function iterates over each row in the game field, checks if the row is
 * full, and if so, shifts the rows above it down and updates the score. The
 * high score is also updated if the current score exceeds it.
 *
 * @return None
 */
static void anihilate() {
  int scoreadd = 0;
  for (short i = 0; i < ROWS_MAP; i++) {
    short itsline = 1;
    for (short j = 0; j < COLS_MAP && itsline; j++)
      if (game_info.field[i][j] != 1) itsline = 0;
    if (itsline) {
      masshift(i--);
      scoreadd = scoreadd * 2 + 100;
    }
  }
  scoreAdd(scoreadd);
  updateHighScore();
}

static void scoreAdd(int scoreadd) {
  static int last_score;
  game_info.score += scoreadd;
  if (game_info.score - last_score > LEVEL_UP_SCORE && game_info.level < 10) {
    game_info.level++;
    last_score = game_info.score;
  }
  game_info.speed = game_info.level;
}

static void updateHighScore() {
  if (game_info.score > game_info.high_score) {
    game_info.high_score = game_info.score;
    FILE *hsfile = fopen(HS_FILE, "w");
    if (hsfile) {
      fprintf(hsfile, "%d", game_info.high_score);
      fclose(hsfile);
    }
  }
}

static void masshift(int start) {
  for (short i = start; i < ROWS_MAP - 1; i++)
    for (short j = 0; j < COLS_MAP; j++)
      if (game_info.field[i][j] <= 1)
        game_info.field[i][j] = game_info.field[i + 1][j];
}

/// @brief Check if figure neigbors are free
/// @param dir 1 - left, 0 down, -1 - right
/// @return 1 if there is free space, 0 if not
static short checkBounds(short dir) {
  short ret = 1;
  for (short j = 0; j < COLS_MAP && ret; j++)
    for (short i = 0; i < ROWS_MAP + FIGURE_H && ret; i++)
      ret = !(game_info.field[i][j] > 1 &&
              (i - !dir < 0 || j - dir < 0 || j - dir >= COLS_MAP ||
               i - !dir >= ROWS_MAP + FIGURE_H ||
               game_info.field[i - !dir][j - dir] == 1));
  return ret;
}

static void doexit() {
  if (game_info.field) {
    for (short i = 0; i < ROWS_MAP + FIGURE_H; i++)
      if (game_info.field[i]) free(game_info.field[i]);
    free(game_info.field);
    game_info.field = NULL;
  }
  if (game_info.next) {
    for (short i = 0; i < FIGURE_H; i++)
      if (game_info.next[i]) free(game_info.next[i]);
    free(game_info.next);
    game_info.next = NULL;
  }
  game_info.speed = 0;
  game_info.level = 0;
  exitstate_sw();
}

static void restart() {
  if (game_info.field)
    for (short i = 0; i < ROWS_MAP + FIGURE_H; i++)
      for (short j = 0; j < COLS_MAP; j++) game_info.field[i][j] = 0;
  if (game_info.next)
    for (short i = 0; i < FIGURE_H; i++)
      for (short j = 0; j < FIGURE_W; j++) game_info.next[i][j] = 0;
  start();
  spawn_sw();
}
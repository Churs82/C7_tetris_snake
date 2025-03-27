#include "fsm.h"

#include <sys/time.h>
#include <time.h>

#include "libsnake.h"

static GameInfo_t* game_info() { static GameInfo_t game_info; return &game_info; }
static game_state* state() { static game_state state; return &state; }
static Direction_t* dir() { static Direction_t dir = Left; return &dir; }

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

static void pausetoggle() { game_info()->pause = !game_info()->pause; }

static void spawn_sw() { *state() = SPAWN; }

static void down_sw() { *state() = DOWN_SHIFTING; }

static void attach_sw() { *state() = ATTACHING; }

static void gameover_sw() { *state() = GAME_OVER; }

static void move_sw() { *state() = MOVING; }

static void exitstate_sw() { *state() = EXIT_STATE; }

void _userAction(UserAction_t action) {
  if (game_info()->pause) checkTime();
  if (!game_info()->pause || action == Pause)
    if (FSM_TABLE[*state()][action]) FSM_TABLE[*state()][action]();
}

GameInfo_t _updateCurrentState() {
  if (game_info()->speed == 0 && *state() != EXIT_STATE) start();
  if (!game_info()->pause && FSM_TRANSFER[*state()]) FSM_TRANSFER[*state()]();
  return *game_info();
}

game_state _getState() { return *state(); }

static void checkTime() {
  static struct timeval last_time;
  struct timeval current_time;
  gettimeofday(&current_time, NULL);

  if (!last_time.tv_sec || game_info()->pause ||
      last_time.tv_sec > current_time.tv_sec)
    last_time = current_time;
  else if (((current_time.tv_sec * CLOCKS_PER_SEC + current_time.tv_usec) -
            (last_time.tv_sec * CLOCKS_PER_SEC + last_time.tv_usec)) >=
           CLOCKS_PER_SEC * 2 / game_info()->speed) {
    down_sw();
    last_time = current_time;
  }
}

/**
 * @brief Initialize game state.
 *
 * Sets speed, score, pause state, and level to their starting values.
 * If the game field is not yet allocated, allocate it.
 * Set the game state to START.
 * Read the high score from the file specified in HS_FILE.
 */
static void start() {
  game_info()->speed = 1;
  game_info()->score = 0;
  game_info()->pause = 0;
  game_info()->level = 1;
  srand(time(NULL));

  if (!game_info()->field) {
    game_info()->field = (int **)calloc(ROWS_MAP, sizeof(int *));
    for (int i = 0; i < ROWS_MAP; i++)
      game_info()->field[i] = (int *)calloc(COLS_MAP, sizeof(int));
  }
  *state() = START;

  FILE *hsfile = fopen(HS_FILE, "r");
  if (hsfile) {
    fscanf(hsfile, "%d", &game_info()->high_score);
    fclose(hsfile);
  }
}

/**
 * @brief Spawns a new Apple on the game field.
 *
 * This function selects a random position on the game field and checks if it's 
 * occupied. If the position is occupied, it recursively tries another position.
 * Once an unoccupied position is found, it places an Apple there and transitions 
 * the game state to MOVING.
 */
static void spawn() {
  int bibidibabidiboo = rand() % (COLS_MAP * ROWS_MAP);
  if (game_info()->field[bibidibabidiboo / COLS_MAP][bibidibabidiboo % COLS_MAP]) spawn();
  else game_info()->field[bibidibabidiboo / COLS_MAP][bibidibabidiboo % COLS_MAP] = 1;
  move_sw();
}

/**
 * @brief Initialize Snake on the game field.
 *
 * Puts the initial snake head at the center of the map and the rest of the snake
 * to the right of the head. The length of the snake is determined by INITIAL_LEN.
 * After placing the snake, it transitions the game state to SPAWN so that an Apple
 * can be spawned.
 */
static void putSnake(){
  short start_h = ROWS_MAP / 2 - 1;
  short start_w = COLS_MAP / 2 - INITIAL_LEN / 2;
  game_info()->field[start_h][start_w] = 3;
  for (short i = 1; i <= INITIAL_LEN; i++) 
    game_info()->field[start_h][start_w+i] = 2;
  spawn_sw();
}


static void move() {
  for (short i = 0; i < ROWS_MAP; i++)
    for (short j = COLS_MAP - 1; j >= 0; j--)
        if (game_info()->field[i][j] > 2) {
          short last_i = i;
          short last_j = j;

        }
  else
    gameover_sw();
}

static void rotate() {
  int right = 0, top = 0, bottom = ROWS_MAP, left = COLS_MAP;
  findBounds(&left, &top, &right, &bottom);
  short possibility = 1;
  int color = 0;
  for (short i = bottom; i <= top && possibility; i++)
    for (short j = left; j <= right && possibility; j++)
      if (game_info()->field[i][j] > 1) {
        if (!color) color = game_info()->field[i][j];
        int new_row = top - (j - left);
        int new_col = left + (i - bottom);
        if (new_col < 0 || new_row < 0 || new_col >= COLS_MAP ||
            new_row >= ROWS_MAP ||
            game_info()->field[new_row][new_col] == 1)
          possibility = 0;
        else if (game_info()->field[new_row][new_col] == 0)
          game_info()->field[new_row][new_col] = -color;
        else if (game_info()->field[new_row][new_col] == color)
          game_info()->field[new_row][new_col]++;
      }
  for (short i = 0; i < ROWS_MAP; i++)
    for (short j = 0; j < COLS_MAP; j++)
      if (game_info()->field[i][j] < 0)
        game_info()->field[i][j] = possibility * color;
      else if (game_info()->field[i][j] > color)
        game_info()->field[i][j] = color;
      else if (game_info()->field[i][j] == color)
        game_info()->field[i][j] = possibility ? 0 : color;
}

static void findBounds(int *left, int *top, int *right, int *bottom) {
  for (short j = 0; j < COLS_MAP; j++)
    for (short i = 0; i < ROWS_MAP; i++)
      if (game_info()->field[i][j] > 1) {
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
      for (short i = 0; i < ROWS_MAP; i++) {
        if (game_info()->field[i][j] > 1) {
          game_info()->field[i - 1][j] = game_info()->field[i][j];
          game_info()->field[i][j] = 0;
          move_sw();
        }
      }
    }
  }
}

static void attach() {
  for (short i = 0; i < ROWS_MAP; i++)
    for (short j = 0; j < COLS_MAP; j++)
      if (game_info()->field[i][j] > 1) game_info()->field[i][j] = 1;
  anihilate();
  spawn_sw();
  for (short j = 0; j < COLS_MAP; j++)
    if (game_info()->field[ROWS_MAP - 1][j] == 1) gameover_sw();
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
      if (game_info()->field[i][j] != 1) itsline = 0;
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
  game_info()->score += scoreadd;
  if (game_info()->score - last_score > LEVEL_UP_SCORE && game_info()->level < 10) {
    game_info()->level++;
    last_score = game_info()->score;
  }
  game_info()->speed = game_info()->level;
}

static void updateHighScore() {
  if (game_info()->score > game_info()->high_score) {
    game_info()->high_score = game_info()->score;
    FILE *hsfile = fopen(HS_FILE, "w");
    if (hsfile) {
      fprintf(hsfile, "%d", game_info()->high_score);
      fclose(hsfile);
    }
  }
}

static void masshift(int start) {
  for (short i = start; i < ROWS_MAP - 1; i++)
    for (short j = 0; j < COLS_MAP; j++)
      if (game_info()->field[i][j] <= 1)
        game_info()->field[i][j] = game_info()->field[i + 1][j];
}

/// @brief Check if figure neigbors are free
/// @param dir 1 - left, 0 down, -1 - right
/// @return 1 if there is free space, 0 if not
static short checkBounds(short dir) {
  short ret = 1;
  for (short j = 0; j < COLS_MAP && ret; j++)
    for (short i = 0; i < ROWS_MAP && ret; i++)
      ret = !(game_info()->field[i][j] > 1 &&
              (i - !dir < 0 || j - dir < 0 || j - dir >= COLS_MAP ||
               i - !dir >= ROWS_MAP ||
               game_info()->field[i - !dir][j - dir] == 1));
  return ret;
}

static void doexit() {
  if (game_info()->field) {
    for (short i = 0; i < ROWS_MAP; i++)
      if (game_info()->field[i]) free(game_info()->field[i]);
    free(game_info()->field);
    game_info()->field = NULL;
  }

  game_info()->speed = 0;
  game_info()->level = 0;
  exitstate_sw();
}

static void restart() {
  if (game_info()->field)
    for (short i = 0; i < ROWS_MAP; i++)
      for (short j = 0; j < COLS_MAP; j++) game_info()->field[i][j] = 0;
  // TODO starting snake position
  start();
  spawn_sw();
}
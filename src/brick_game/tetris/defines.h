#ifndef DEFINES_H
#define DEFINES_H

#define ROWS_MAP 20
#define COLS_MAP 10

#define SUCCESS 0
#define ERROR 1

#define FIGURES_COUNT 7
#define FIGURES                                                               \
  (int[FIGURE_H * FIGURES_COUNT][FIGURE_W]) {                                 \
    {0, 2, 0, 0}, {0, 2, 0, 0}, {0, 2, 0, 0}, {0, 2, 0, 0}, {0, 0, 0, 0},     \
        {0, 0, 0, 0}, {0, 3, 3, 0}, {0, 3, 3, 0}, {0, 0, 0, 0}, {0, 4, 4, 0}, \
        {0, 4, 0, 0}, {0, 4, 0, 0}, {0, 0, 0, 0}, {0, 5, 5, 0}, {0, 0, 5, 0}, \
        {0, 0, 5, 0}, {0, 0, 0, 0}, {0, 0, 6, 0}, {0, 6, 6, 0}, {0, 6, 0, 0}, \
        {0, 0, 0, 0}, {0, 7, 0, 0}, {0, 7, 7, 0}, {0, 0, 7, 0}, {0, 0, 0, 0}, \
        {0, 0, 0, 0}, {0, 8, 8, 8}, {                                         \
      0, 0, 8, 0                                                              \
    }                                                                         \
  }
#define LEVEL_UP_SCORE 600
#define HS_FILE "~/games/tetris_hs.conf"

#endif
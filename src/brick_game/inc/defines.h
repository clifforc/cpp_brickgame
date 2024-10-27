/**
 * @file defines.h
 * @brief Contains the definitions of various constants used in the Tetris game.
 */

#pragma once

#include <algorithm>
#include <memory>
#include <cstring>
#include <cstddef>

/**
 * @struct Figure
 * @brief Structure representing a tetris figure.
 * @var Figure.x x-coordinate of the figure's position.
 * @var Figure.y y-coordinate of the figure's position.
 * @var Figure.figure_num Number representing the type of figure.
 * @var Figure.figure 2D array representing the shape of the figure.
 */
typedef struct Figure {
  int x;
  int y;
  int figure_num;
  int **figure;
} Figure;

/**
 * @struct GameInfo_t
 * @brief Structure representing the game state and information.
 * @var GameInfo_t.field 2D array representing the game field.
 * @var GameInfo_t.next_figure Pointer to the next figure.
 * @var GameInfo_t.score Current score.
 * @var GameInfo_t.high_score Highest score achieved.
 * @var GameInfo_t.level Current level.
 * @var GameInfo_t.speed Current speed of the game.
 * @var GameInfo_t.pause Flag indicating if the game is paused.
 * @var GameInfo_t.figure Pointer to the current figure.
 * @var GameInfo_t.status Current status of the game.
 * @var GameInfo_t.action Current action being performed.
 * @var GameInfo_t.ticks_left Number of ticks left for the current action.
 */
typedef struct {
  int **field;
  Figure *next_figure;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;

  Figure *figure;
  int status;
  int action;
  int ticks_left;
} GameInfo_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  IDLE
} UserAction_t;

typedef enum { Running, Paused, GameOver, Exit, Win } GameState;

#define TICKS_START 30 /**< Initial number of ticks. */

#define FIGURE_SIZE 5   /**< Size of tetris figure. */
#define FIGURES_COUNT 7 /**< Total number of tetris figures. */

#define WIDTH_FACTOR 2 /**< Factor used to calculate width of game field.*/

#define FIELD_WIDTH 10  /**< Width of the game field. */
#define FIELD_HEIGHT 20 /**< Height of the game field. */
#define FIELD_BORDERS 2 /**< Number of border lines around the game field. */

#define FIELD_START_X 17 /**< Starting x-coordinate of the game field. */
#define FIELD_START_Y 2  /**< Starting y-coordinate of the game field. */

#define NEXT_FIELD_WIDTH 12 /**< Width of the next figure field. */
#define NEXT_FIELD_HEIGHT 7 /**< Height of the next figure field. */
#define NEXT_FIELD_X 0      /**< x-coordinate of the next figure field. */
#define NEXT_FIELD_Y 10     /**< y-coordinate of the next figure field. */

#define FIGURE_START_X                 \
  (((FIELD_WIDTH - FIGURE_SIZE) / 2) + \
   1)                    /**< Starting x-coordinate of each Tetris figure. */
#define FIGURE_START_Y 0 /**< Starting y-coordinate of each Tetris figure. */

#define BASE_SPEED 3000000 /**< Base speed to decrese speed for each level. */

#define MAX_LEVEL 10
#define POINTS_PER_LEVEL 5
#define BASE_SPEED_S 500
#define MAX_SPEED 100

#define HEX_WHITE "#FFFFFF"
#define HEX_ORANGE "#FF8D1A"
#define HEX_RED "#F94144"
#define HEX_GREEN "#03C03C"
#define HEX_YELLOW "#f6e000"
#define HEX_BLUE "#0096FF"
#define HEX_MAGENTA "#D6006E"
#define HEX_CYAN "#00CED1"

#define ORANGE_COLOR 1
#define RED_COLOR 2
#define GREEN_COLOR 3
#define YELLOW_COLOR 4
#define BLUE_COLOR 5
#define MAGENTA_COLOR 6
#define CYAN_COLOR 7

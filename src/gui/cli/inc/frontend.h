/**
 * @file frontend.h
 * @brief Header file containing functions and macros for the Tetris game
 * frontend.
 */

#ifndef FRONTEND_H
#define FRONTEND_H

#include <ncurses.h>

#include "../../../brick_game/inc/defines.h"
#include "../../../brick_game/tetris/inc/backend.h"
#include "../../../brick_game/tetris/inc/fsm_t.h"

/**
 * @brief Initializes the ncruses library.
 *
 * This function initializes the ncruses library and activate some lib modules.
 *
 */
void win_init();

/**
 * @brief Initializes color pairs.
 *
 * This function initializes color pairs used for tetris figures.
 *
 */
void color_init();

/**
 * @brief Creates a new window with the specified dimensions and position.
 *
 * @param height Height of the window.
 * @param width Width of the window.
 * @param starty Starting y-coordinate of the window.
 * @param startx Starting x-coordinate of the window.
 * @return Pointer to the newly created window.
 */
WINDOW *create_newwin(int height, int width, int starty, int startx);

/**
 * @brief Displays the game text.
 *
 * This function displays game text such as level, score, controls etc.
 *
 * @param game Pointer to the GameInfo_t struct containing the game information.
 */
void game_field_text(GameInfo_t *game);

/**
 * @brief Draws the game field in the specified window.
 *
 * This function draws the game field in the specified ncurses window.
 *
 * @param win Pointer to the ncurses window.
 * @param game Pointer to the GameInfo_t struct containing the game information.
 */
void draw_game_field(WINDOW *win, GameInfo_t *game);

/**
 * @brief Draws the next figure in the specified window.
 *
 * This function draws the next figure in the specified ncurses window.
 *
 * @param win Pointer to the ncurses window.
 * @param game Pointer to the GameInfo_t struct containing the game information.
 */
void draw_next_figure(WINDOW *win, GameInfo_t *game);

/**
 * @brief Displays the pause text in the game.
 *
 * This function displays the pause text in the game while pause state.
 *
 */
void pause_text();

/**
 * @brief Displays the gameover text in the game.
 *
 * This function displays the game over text in the game while gameover state.
 *
 */
void gameover_text();

/**
 * @brief Displays the win text in the game.
 *
 * This function displays the win text in the game while win state.
 *
 */
void win_text();

/**
 * @brief Clears the specified window.
 *
 * This function hides figures from field while pause or gameover state.
 *
 * @param win A pointer to the ncurses window.
 */
void clear_win(WINDOW *win);

/**
 * @brief Displays the menu to choose game.
 *
 * This function displays the choose game menu at start.
 *
 */
int show_menu();

/**
 * @brief Main tetris gameloop.
 *
 * This function starts tetris game.
 *
 */
void game_loop_tetris();

/**
 * @brief Main snake gameloop.
 *
 * This function starts snake game.
 *
 */
void game_loop_snake();

#endif

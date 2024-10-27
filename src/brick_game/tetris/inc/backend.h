/**
 * @file backend.h
 * @brief Header file containing declarations for brick_game (aka tetris)
 * backend.
 */

#ifndef BACKEND_H
#define BACKEND_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../inc/defines.h"

/**
 * @brief Enumeration of game states.
 *
 * This enumeration defines game states for the Tetris game.
 * - GAMEOVER: Represents the game over state.
 * - RESET: Represents the reset state.
 */
enum {
  GAMEOVER = 10,
  RESET,
};

/**
 * @brief Initializes the game state and returns a pointer to the GameInfo_t
 * structure.
 * @return Pointer to the initialized GameInfo_t structure.
 */
GameInfo_t *game_init();

/**
 * @brief Frees the memory allocated for the GameInfo_t structure.
 * @param game Pointer to the GameInfo_t structure.
 */
void free_game_init(GameInfo_t *game);

/**
 * @brief Initializes the game field and returns a pointer to the 2D array
 * representing the field.
 * @return Pointer to the initialized game field.
 */
int **init_game_field();

/**
 * @brief Frees the memory allocated for the game field.
 * @param field Pointer to the game field.
 */
void free_game_field(int **field);

/**
 * @brief Initializes a new figure with the given coordinates and returns a
 * pointer to the Figure structure.
 * @param figure_x x-coordinate of the figure's position.
 * @param figure_y y-coordinate of the figure's position.
 * @return Pointer to the initialized Figure structure.
 */
Figure *init_figure(int figure_x, int figure_y);

/**
 * @brief Frees the memory allocated for the Figure structure.
 * @param figure Pointer to the Figure structure.
 */
void free_figure(Figure *figure);

/**
 * @brief Generates a random figure and assigns it to the given Figure
 * structure.
 * @param figure Pointer to the Figure structure.
 */
void get_random_figure(Figure *figure);

/**
 * @brief Places the current figure on the game field.
 * @param game Pointer to the GameInfo_t structure.
 */
void place_figure_on_field(GameInfo_t *game);

/**
 * @brief Clears the current figure from the game field.
 * @param game Pointer to the GameInfo_t structure.
 */
void clear_figure_from_field(GameInfo_t *game);

/**
 * @brief Checks if there is a collision between the current figure and the game
 * field.
 * @param game Pointer to the GameInfo_t structure.
 * @return 1 if there is a collision, 0 otherwise.
 */
int collision(GameInfo_t *game);

/**
 * @brief Drops filled lines and updates the game state.
 * @param i Index of the filled line.
 * @param game Pointer to the GameInfo_t structure.
 */
void drop_filled_lines(int i, GameInfo_t *game);

/**
 * @brief Erases filled lines and updates the score.
 * @param game Pointer to the GameInfo_t structure.
 */
void erase_and_score(GameInfo_t *game);

/**
 * @brief Checks if a line is filled with blocks.
 * @param i Index of the line to check.
 * @param game Pointer to the GameInfo_t structure.
 * @return 1 if the line is filled, 0 otherwise.
 */
int check_filled_line(int i, const GameInfo_t *game);

/**
 * @brief Checks for collision, updates the score, and performs necessary
 * actions after placing a figure on the game field.
 * @param game Pointer to the GameInfo_t structure.
 */
void plant_check_collision_and_score(GameInfo_t *game);

/**
 * @brief Checks the number of ticks left for the current action and performs
 * necessary actions when the ticks reach zero.
 * @param game Pointer to the GameInfo_t structure.
 */
void check_ticks(GameInfo_t *game);

/**
 * @brief Calculates the current level based on the score and updates the game
 * state.
 * @param game Pointer to the GameInfo_t structure.
 */
void calculate_level(GameInfo_t *game);

/**
 * @brief Calculates the current speed based on the level and updates the game
 * state.
 * @param game Pointer to the GameInfo_t structure.
 */
void calculate_speed(GameInfo_t *game);

/**
 * @brief Loads the highest score from a file.
 * @return Highest score.
 */
int load_score();

/**
 * @brief Updates the highest score if the current score is higher.
 * @param game Pointer to the GameInfo_t structure.
 */
void update_max_score(GameInfo_t *game);

/**
 * @brief Saves the highest score to a file.
 * @param game Pointer to the GameInfo_t structure.
 */
void save_max_score(const GameInfo_t *game);

#endif

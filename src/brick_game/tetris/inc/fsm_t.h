/**
 * @file fsm.h
 * @brief Header file containing the declaration of the finite state machine
 * functions and types.
 */

#ifndef FSM_T_H
#define FSM_T_H

#include "backend.h"

/**
 * @brief Calculates the game state based on the user actions.
 * @param game The game information.
 */
void calculate_game(GameInfo_t* game);

/**
 * @brief Performs rotate action.
 * @param game The game information.
 */
void action_up(GameInfo_t* game);

/**
 * @brief Performs move left action.
 * @param game The game information.
 */
void action_left(GameInfo_t* game);

/**
 * @brief Performs move right action.
 * @param game The game information.
 */
void action_right(GameInfo_t* game);

/**
 * @brief Performs move down action.
 * @param game The game information.
 */
void action_down(GameInfo_t* game);

/**
 * @brief Performs drop fogure down action.
 * @param game The game information.
 */
void action_drop(GameInfo_t* game);

/**
 * @brief Performs pause action.
 * @param game The game information.
 */
void action_pause(GameInfo_t* game);

/**
 * @brief Performs start action.
 * @param game The game information.
 */
void action_start(GameInfo_t* game);

/**
 * @brief Gets the user action.
 * @param game The game information.
 */
void get_user_action(GameInfo_t* game, int ch);

/**
 * @brief Moves the current figure down.
 * @param game The game information.
 */
void move_down(GameInfo_t* game);

/**
 * @brief Moves the current figure up.
 * @param game The game information.
 */
void move_up(GameInfo_t* game);

/**
 * @brief Moves the current figure right.
 * @param game The game information.
 */
void move_right(GameInfo_t* game);

/**
 * @brief Moves the current figure left.
 * @param game The game information.
 */
void move_left(GameInfo_t* game);

/**
 * @brief Rotates the current figure.
 * @param game The game information.
 * @return Pointer to the rotated figure.
 */
Figure* rotate(GameInfo_t* game);

/**
 * @brief Plants the current figure on the game board.
 * @param game The game information.
 */
void plant_figure(GameInfo_t* game);

/**
 * @brief Spawns next figure and create new next figure preview.
 * @param game The game information.
 */
void spawn_new(GameInfo_t* game);

#endif
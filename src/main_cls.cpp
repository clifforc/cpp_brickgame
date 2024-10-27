#include "./gui/cli/inc/frontend.h"

/**
 * @brief Main function of the tetris game.
 *
 * This function initializes the game window, sets up the colors
 * starts the game loop, and cleans up the window before exiting.
 *
 * @return 0 indicating successful execution of the program.
 */

int main() {
  win_init();
  color_init();
  srand(time(NULL));

  int choice = show_menu();
  if (choice == 1) {
    game_loop_tetris();
  } else if (choice == 2) {
    game_loop_snake();
  }

  endwin();
  return 0;
}

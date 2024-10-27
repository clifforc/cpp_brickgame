#include "../../brick_game/snake/controller/inc/game_controller.h"
#include "inc/frontend.h"

#include <ncurses.h>

s21::GameModel game_model;
s21::GameController GameController(&game_model);

void game_loop_snake() {
  GameController.userInput(Start, false);
  WINDOW *main_win = create_newwin(FIELD_HEIGHT + FIELD_BORDERS,
                                   FIELD_WIDTH * WIDTH_FACTOR + FIELD_BORDERS,
                                   FIELD_START_Y, FIELD_START_X);
  mvprintw(1, 23, "S N A K E");
  bool running = true;
  bool act = false;
  while (running) {
    int ch = getch();
    switch (ch) {
    case KEY_LEFT:
      GameController.userInput(Left, false);
      break;
    case KEY_RIGHT:
      GameController.userInput(Right, false);
      break;
    case KEY_UP:
      GameController.userInput(Up, false);
      break;
    case KEY_DOWN:
      GameController.userInput(Down, false);
      break;
    case 'x':
    case 'X':
      act = !act;
      GameController.userInput(Action, act);
      break;
    case 'p':
    case 'P':
      GameController.userInput(Pause, false);
      break;
    case 'q':
    case 'Q':
      GameController.userInput(Terminate, false);
      running = false;
      break;
    case '\n':
      GameController.userInput(Start, false);
      break;
    default:
      break;
    }

    if (!running) {
      break;
    }

    GameInfo_t game_info = game_model.UpdateCurrentState();

    game_field_text(&game_info);
    draw_game_field(main_win, &game_info);

    GameState state = game_model.GetGameState();
    if (state == Paused) {
      pause_text();
    } else if (state == GameOver) {
      clear_win(main_win);
      gameover_text();
      if (ch == '\n') {
        GameController.userInput(Start, false);
      }
    } else if (state == Win) {
      clear_win(main_win);
      win_text();
      if (ch == '\n') {
        GameController.userInput(Start, false);
      }
    }
    napms(50);
  }
  delwin(main_win);
}

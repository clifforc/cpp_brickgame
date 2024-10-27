#include "inc/frontend.h"

void game_loop_tetris() {
  struct timespec sp_start, sp_end, ts1, ts2 = {0, 0};
  GameInfo_t* game = game_init();
  WINDOW* main_win;
  WINDOW* next_figure_win;
  main_win = create_newwin(FIELD_HEIGHT + FIELD_BORDERS,
                           FIELD_WIDTH * WIDTH_FACTOR + FIELD_BORDERS,
                           FIELD_START_Y, FIELD_START_X);
  next_figure_win = create_newwin(NEXT_FIELD_HEIGHT, NEXT_FIELD_WIDTH,
                                  NEXT_FIELD_Y, NEXT_FIELD_X);
  mvprintw(1, 22, "T E T R I S");
  spawn_new(game);
  while (game->status != Terminate) {
    clock_gettime(CLOCK_MONOTONIC, &sp_start);
    int ch = getch();
    get_user_action(game, ch);
    calculate_game(game);
    if (game->status != Pause && game->status != GAMEOVER) {
      place_figure_on_field(game);
      game_field_text(game);
      draw_game_field(main_win, game);
      clear_figure_from_field(game);
      draw_next_figure(next_figure_win, game);
      refresh();
    } else if (game->status == Pause) {
      game_field_text(game);
      draw_game_field(main_win, game);
      clear_win(main_win);
      pause_text();
    } else if (game->status == GAMEOVER) {
      clear_win(main_win);
      gameover_text();
    }
    if (game->status == RESET) {
      free_game_init(game);
      game = game_init();
      game->status = Start;
    }
    clock_gettime(CLOCK_MONOTONIC, &sp_end);
    if (sp_end.tv_sec - sp_start.tv_sec <= 0 &&
        (ts2.tv_nsec = 33000000 - game->speed -
                       (sp_end.tv_nsec - sp_start.tv_nsec)) > 0) {
      nanosleep(&ts2, &ts1);
    }
  }
  free_game_init(game);
}
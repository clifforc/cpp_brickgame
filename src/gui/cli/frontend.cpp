#include "./inc/frontend.h"

WINDOW *create_newwin(int height, int width, int starty, int startx) {
  WINDOW *main_win;
  main_win = newwin(height, width, starty, startx);
  box(main_win, 0, 0);
  wrefresh(main_win);
  return main_win;
}

void win_init() {
  initscr();
  noecho();
  curs_set(0);
  nodelay(stdscr, TRUE);
  cbreak();
  keypad(stdscr, TRUE);
}

void color_init() {
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_WHITE);
  init_pair(2, COLOR_RED, COLOR_RED);
  init_pair(3, COLOR_GREEN, COLOR_GREEN);
  init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(5, COLOR_BLUE, COLOR_BLUE);
  init_pair(6, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(7, COLOR_CYAN, COLOR_CYAN);
}

void game_field_text(GameInfo_t *game) {
  int l_align = 0;
  int r_align = 45;

  move(3, l_align);
  clrtoeol();
  move(5, l_align);
  clrtoeol();
  move(7, l_align);
  clrtoeol();

  mvprintw(3, l_align, "LEVEL: %d", game->level);
  mvprintw(5, l_align, "SCORE: %d", game->score);
  mvprintw(7, l_align, "MAX SCORE: %d", game->high_score);
  mvprintw(3, r_align, "LEFT ARROW  : MOVE LEFT");
  mvprintw(5, r_align, "RIGHT ARROW : MOVE RIGHT");
  mvprintw(7, r_align, "DOWN ARROW  : MOVE DOWN");
  mvprintw(9, r_align, "UP ARROW    : MOVE UP/ROTATE");
  mvprintw(11, r_align, "X : ACTION");
  mvprintw(13, r_align, "P : PAUSE");
  mvprintw(15, r_align, "Q : EXIT");

  refresh();
}

void draw_game_field(WINDOW *win, GameInfo_t *game) {
  box(win, 0, 0);
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++)
      for (int k = 0; k < 2; k++) {
        wattron(win, COLOR_PAIR(game->field[i][j]));
        mvwaddch(win, 1 + i, 1 + j * WIDTH_FACTOR + k, ' ');
        wattroff(win, COLOR_PAIR(game->field[i][j]));
      }
  wrefresh(win);
}

void draw_next_figure(WINDOW *win, GameInfo_t *game) {
  for (int i = 0; i < FIGURE_SIZE; i++)
    for (int j = 0; j < FIGURE_SIZE; j++)
      for (int k = 0; k < 2; k++) {
        wattron(win, COLOR_PAIR(game->next_figure->figure[i][j]));
        mvwaddch(win, 1 + i, 1 + j * WIDTH_FACTOR + k, ' ');
        wattroff(win, COLOR_PAIR(game->next_figure->figure[i][j]));
      }
  wrefresh(win);
}

void pause_text() {
  mvprintw(11, 22, "Press ENTER");
  mvprintw(12, 22, "to Start");
  refresh();
}

void gameover_text() {
  mvprintw(11, 22, "GAME OVER");
  mvprintw(12, 22, "Press ENTER");
  mvprintw(13, 22, "to Restart");
  refresh();
}

void win_text() {
  mvprintw(11, 22, "YOU WIN!");
  mvprintw(12, 22, "Press ENTER");
  mvprintw(13, 22, "to Restart");
  refresh();
}

void clear_win(WINDOW *win) {
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++)
      for (int k = 0; k < 2; k++) {
        mvwaddch(win, 1 + i, 1 + j * WIDTH_FACTOR + k, ' ');
      }
  wrefresh(win);
}

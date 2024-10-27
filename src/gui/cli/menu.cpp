#include "inc/frontend.h"

int show_menu() {
  int choice = 0;
  while (choice != 1 && choice != 2){
    clear();
    mvprintw(10, 20, "Select Game:");
    mvprintw(12, 22, "1. Tetris");
    mvprintw(14, 22, "2. Snake");
    refresh();
    noecho();
    choice = getch() - '0';
    napms(500);
  }
  return choice;
}
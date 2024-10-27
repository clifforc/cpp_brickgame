#include "./inc/fsm_t.h"

#include "./../../gui/cli/inc/frontend.h"

void calculate_game(GameInfo_t* game) {
  check_ticks(game);
  switch (game->action) {
    case Up:
      action_up(game);
      break;
    case Left:
      action_left(game);
      break;
    case Right:
      action_right(game);
      break;
    case Down:
      action_down(game);
      break;
    case Action:
      action_drop(game);
      break;
    case Pause:
      action_pause(game);
      break;
    case Terminate:
      game->status = Terminate;
      break;
    case Start:
      action_start(game);
      break;
    case IDLE:
    default:
      break;
  }
  game->action = IDLE;
  if (game->status != Pause && game->status != GAMEOVER)
    game->ticks_left--;
  else
    game->ticks_left = TICKS_START;
}

void action_up(GameInfo_t* game) {
  Figure* figure = rotate(game);
  Figure* old_figure = game->figure;
  game->figure = figure;
  if (collision(game)) game->figure->x -= 1;
  if (collision(game)) game->figure->x -= 1;
  if (collision(game)) game->figure->x += 3;
  if (collision(game)) game->figure->x += 1;
  if (collision(game)) {
    game->figure = old_figure;
    free_figure(figure);
  } else {
    free_figure(old_figure);
  }
}

void action_left(GameInfo_t* game) {
  move_left(game);
  if (collision(game)) move_right(game);
}

void action_right(GameInfo_t* game) {
  move_right(game);
  if (collision(game)) move_left(game);
}

void action_down(GameInfo_t* game) {
  move_down(game);
  if (collision(game)) move_up(game);
}

void action_drop(GameInfo_t* game) {
  while (!collision(game)) {
    move_down(game);
    if (collision(game)) {
      move_up(game);
      plant_check_collision_and_score(game);
      break;
    }
  }
}

void action_pause(GameInfo_t* game) { game->status = Pause; }

void action_start(GameInfo_t* game) {
  if (game->status == GAMEOVER) {
    game->status = RESET;
  } else {
    game->status = Start;
  }
}

void get_user_action(GameInfo_t* game, int ch) {
  if (game->status != Pause) {
    switch (ch) {
      case KEY_UP:
        game->action = Up;
        break;
      case KEY_LEFT:
        game->action = Left;
        break;
      case KEY_RIGHT:
        game->action = Right;
        break;
      case KEY_DOWN:
        game->action = Down;
        break;
      case 'x':
      case 'X':
        game->action = Action;
        break;
      default:
        game->action = IDLE;
        break;
    }
  }
  switch (ch) {
    case 'p':
    case 'P':
      game->action = Pause;
      break;
    case 'q':
    case 'Q':
      game->action = Terminate;
      break;
    case '\n':
      game->action = Start;
      break;
  }
}

void move_down(GameInfo_t* game) { game->figure->y++; }

void move_up(GameInfo_t* game) { game->figure->y--; }

void move_right(GameInfo_t* game) { game->figure->x++; }

void move_left(GameInfo_t* game) { game->figure->x--; }

Figure* rotate(GameInfo_t* game) {
  Figure* figure = init_figure(FIGURE_START_X, FIGURE_START_Y);
  Figure* figure_old = game->figure;

  figure->x = figure_old->x;
  figure->y = figure_old->y;
  figure->figure_num = figure_old->figure_num;

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (figure->figure_num == 6)
        figure->figure[j][FIGURE_SIZE - 1 - i] =
            j == 0 ? 0 : figure_old->figure[i][j - 1];
      else if (figure->figure_num == 5)
        figure->figure[i][j] = figure_old->figure[i][j];
      else
        figure->figure[j][FIGURE_SIZE - 1 - i] =
            (j == 0 || j == 1) ? 0 : figure_old->figure[i][j - 2];
    }
  }
  return figure;
}

void plant_figure(GameInfo_t* game) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (game->figure->figure[i][j] != 0) {
        int field_x = game->figure->x + j;
        int field_y = game->figure->y + i - 2;
        game->field[field_y][field_x] = game->figure->figure[i][j];
      }
    }
  }
}

void spawn_new(GameInfo_t* game) {
  if (game->figure != NULL) {
    free_figure(game->figure);
  }
  game->figure = game->next_figure;
  game->figure->x = FIGURE_START_X;
  game->figure->y = FIGURE_START_Y;
  game->next_figure = init_figure(NEXT_FIELD_X, NEXT_FIELD_Y);
  get_random_figure(game->next_figure);
}
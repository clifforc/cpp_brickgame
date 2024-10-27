#include "./inc/backend.h"

#include "../../gui/cli/inc/frontend.h"

GameInfo_t* game_init() {
  GameInfo_t* game = (GameInfo_t*)malloc(sizeof(GameInfo_t));

  game->field = init_game_field();
  game->next_figure = init_figure(NEXT_FIELD_X, NEXT_FIELD_Y);
  game->score = 0;
  game->high_score = load_score();
  game->level = 1;
  game->speed = 0;

  game->figure = init_figure(FIGURE_START_X, FIGURE_START_Y);
  game->status = Pause;
  game->action = IDLE;
  game->ticks_left = TICKS_START;

  return game;
}

void free_game_init(GameInfo_t* game) {
  if (game) {
    free_game_field(game->field);
    free_figure(game->next_figure);
    free_figure(game->figure);
    free(game);
  }
}

int** init_game_field() {
  int** field = (int**)calloc(FIELD_HEIGHT, sizeof(int*));
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    field[i] = (int*)calloc(FIELD_WIDTH, sizeof(int));
  }
  return field;
}

void free_game_field(int** field) {
  if (field) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      if (field[i]) {
        free(field[i]);
      }
    }
    free(field);
  }
}

Figure* init_figure(int figure_x, int figure_y) {
  Figure* f = (Figure*)malloc(sizeof(Figure));

  f->figure_num = rand() % FIGURES_COUNT;

  f->x = figure_x;
  f->y = figure_y;

  f->figure = (int**)calloc(FIGURE_SIZE, sizeof(int*));
  for (int i = 0; i < FIGURE_SIZE; i++)
    f->figure[i] = (int*)calloc(FIGURE_SIZE, sizeof(int));

  get_random_figure(f);

  return f;
}

void free_figure(Figure* figure) {
  if (figure) {
    for (int i = 0; i < FIGURE_SIZE; i++) {
      if (figure->figure[i]) {
        free(figure->figure[i]);
      }
    }
    free(figure->figure);
    free(figure);
  }
}

void get_random_figure(Figure* figure) {
  int figures[7][FIGURE_SIZE][FIGURE_SIZE] = {
      // 0. z
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {1, 1, 0, 0, 0},
       {0, 1, 1, 0, 0},
       {0, 0, 0, 0, 0}},
      // 1. s
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 2, 2, 0, 0},
       {2, 2, 0, 0, 0},
       {0, 0, 0, 0, 0}},
      // 2. T
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 3, 0, 0, 0},
       {3, 3, 3, 0, 0},
       {0, 0, 0, 0, 0}},
      // 3. L
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 4, 0, 0},
       {4, 4, 4, 0, 0},
       {0, 0, 0, 0, 0}},
      // 4. J
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {5, 0, 0, 0, 0},
       {5, 5, 5, 0, 0},
       {0, 0, 0, 0, 0}},
      // 5. square
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {0, 6, 6, 0, 0},
       {0, 6, 6, 0, 0},
       {0, 0, 0, 0, 0}},
      // 6. bar
      {{0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0},
       {7, 7, 7, 7, 0},
       {0, 0, 0, 0, 0},
       {0, 0, 0, 0, 0}},
  };

  for (int i = 0; i < FIGURE_SIZE; i++) {
    memcpy(figure->figure[i], figures[figure->figure_num][i],
           sizeof(int) * FIGURE_SIZE);
  }
}

void place_figure_on_field(GameInfo_t* game) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (game->figure->figure[i][j] != 0) {
        int field_x = game->figure->x + j;
        int field_y = game->figure->y + i - 2;

        if (field_x >= 0 && field_x < FIELD_WIDTH && field_y >= 0 &&
            field_y < FIELD_HEIGHT) {
          game->field[field_y][field_x] = game->figure->figure[i][j];
        }
      }
    }
  }
}

void clear_figure_from_field(GameInfo_t* game) {
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (game->figure->figure[i][j] != 0) {
        int field_x = game->figure->x + j;
        int field_y = game->figure->y + i - 2;

        if (field_x >= 0 && field_x < FIELD_WIDTH && field_y >= 0 &&
            field_y < FIELD_HEIGHT) {
          game->field[field_y][field_x] = 0;
        }
      }
    }
  }
}

int collision(GameInfo_t* game) {
  int flag = 0;
  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      if (game->figure->figure[i][j] != 0) {
        int field_x = game->figure->x + j;
        int field_y = game->figure->y + i - 2;
        if (field_x < 0 || field_x >= FIELD_WIDTH || field_y < 0 ||
            field_y >= FIELD_HEIGHT || game->field[field_y][field_x] != 0) {
          flag = 1;
        }
      }
    }
  }
  return flag;
}

void drop_filled_lines(int i, GameInfo_t* game) {
  if (i == 0) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 0;
    }
  } else {
    for (int k = i; k > 0; k--) {
      for (int j = 0; j < FIELD_WIDTH; j++) {
        game->field[k][j] = game->field[k - 1][j];
      }
    }
  }
}

void erase_and_score(GameInfo_t* game) {
  int count = 0;
  for (int i = FIELD_HEIGHT - 1; i >= 0; i--) {
    while (check_filled_line(i, game)) {
      drop_filled_lines(i, game);
      count++;
    }
  }
  if (count == 1) game->score += 100;
  if (count == 2) game->score += 300;
  if (count == 3) game->score += 700;
  if (count == 4) game->score += 1500;
}

int check_filled_line(int i, const GameInfo_t* game) {
  int flag = 1;
  for (int j = 0; j < FIELD_WIDTH; j++) {
    if (game->field[i][j] == 0) flag = 0;
  }
  return flag;
}

void plant_check_collision_and_score(GameInfo_t* game) {
  plant_figure(game);

  erase_and_score(game);
  update_max_score(game);
  calculate_level(game);
  calculate_speed(game);
  spawn_new(game);

  if (collision(game)) {
    save_max_score(game);
    game->status = GAMEOVER;
  }
}

void check_ticks(GameInfo_t* game) {
  if (game->ticks_left <= 0) {
    game->ticks_left = TICKS_START;
    move_down(game);

    if (collision(game)) {
      move_up(game);
      plant_check_collision_and_score(game);
    }
  }
}

void calculate_level(GameInfo_t* game) {
  while (game->score >= 600 * game->level && game->level < 10) {
    game->level++;
  }
}

void calculate_speed(GameInfo_t* game) {
  game->speed = game->level * BASE_SPEED;
}

int load_score() {
  int max_score = 0;
  FILE* file = fopen("max_score.txt", "r");
  if (file != NULL) {
    fscanf(file, "%d", &max_score);
    fclose(file);
  }
  return max_score;
}

void update_max_score(GameInfo_t* game) {
  if (game->score > game->high_score) game->high_score = game->score;
}

void save_max_score(const GameInfo_t* game) {
  FILE* file = fopen("max_score.txt", "w");
  if (file != NULL) {
    fprintf(file, "%d", game->high_score);
    fclose(file);
  }
}

#include <cstdio>
#include <gtest/gtest.h>
#include <memory>
#include <ncurses.h>
#include <vector>

#include "../brick_game/tetris/inc/backend.h"
#include "../brick_game/tetris/inc/fsm_t.h"

#include "../brick_game/snake/controller/inc/game_controller.h"

namespace s21 {

class GameControllerTest : public ::testing::Test {
protected:
  GameModel model;
  GameController controller;

  GameControllerTest() : controller(&model) {}

  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(GameControllerTest, UserInput_Start) {
  controller.userInput(Start, false);
  EXPECT_EQ(model.GetGameState(), Running);
}

TEST_F(GameControllerTest, UserInput_Pause) {
  controller.userInput(Pause, false);
  EXPECT_EQ(model.GetGameState(), Paused);
}

TEST_F(GameControllerTest, UserInput_Terminate) {
  controller.userInput(Terminate, false);
  EXPECT_EQ(model.GetGameState(), Exit);
}

TEST_F(GameControllerTest, UserInput_Direction) {
  controller.userInput(Start, false);
  controller.userInput(Left, false);
  controller.userInput(Right, false);
  controller.userInput(Up, false);
  controller.userInput(Down, false);
  controller.userInput(Action, true);
  controller.userInput(Action, false);
  EXPECT_EQ(model.GetGameState(), Running);
}

class AppleTest : public ::testing::Test {
protected:
  Apple apple;
  std::mt19937 generator;

  void SetUp() override {
    std::random_device rd;
    generator.seed(rd());
  }
};

TEST_F(AppleTest, SpawnApple_NoOccupiedPositions) {
  std::vector<Position> occupied;
  apple.SpawnApple(occupied);
  Position pos = apple.GetPosition();
  EXPECT_GE(pos.x, 0);
  EXPECT_LT(pos.x, FIELD_WIDTH);
  EXPECT_GE(pos.y, 0);
  EXPECT_LT(pos.y, FIELD_HEIGHT);
}

TEST_F(AppleTest, SpawnApple_WithOccupiedPositions) {
  std::vector<Position> occupied = {Position(1, 1), Position(2, 2),
                                    Position(3, 3)};
  apple.SpawnApple(occupied);
  Position pos = apple.GetPosition();
  bool is_occupied = false;
  for (const auto &o : occupied) {
    if (pos == o) {
      is_occupied = true;
      break;
    }
  }
  EXPECT_FALSE(is_occupied);
}

class GameModelTest : public ::testing::Test {
protected:
  GameModel game_model_;
  Snake snake_;

  void SetUp() override {
    snake_ = Snake();
  }

  void TearDown() override {}
};

TEST_F(GameModelTest, InitialState) {
  EXPECT_EQ(game_model_.GetGameState(), Paused);
  GameInfo_t info = game_model_.UpdateCurrentState();
  EXPECT_EQ(info.pause, 1);
  EXPECT_EQ(info.score, 0);
  EXPECT_EQ(info.level, 1);
  EXPECT_EQ(info.speed, BASE_SPEED_S);
}

TEST_F(GameModelTest, SetGameState_Running) {
  game_model_.SetGameState(Running);
  EXPECT_EQ(game_model_.GetGameState(), Running);
}

TEST_F(GameModelTest, SetGameState_GameOver) {
  game_model_.SetGameState(GameOver);
  EXPECT_EQ(game_model_.GetGameState(), GameOver);
}

TEST_F(GameModelTest, SetSnakeDirection) {
  game_model_.SetSnakeDirection(Direction::left);
  game_model_.SetGameState(Running);
  game_model_.UpdateCurrentState();
}

TEST_F(GameModelTest, CheckCollisions_GameOver) {
  game_model_.SetGameState(Running);
  for (auto i = 0; i < 15; ++i) {
    game_model_.UpdateGame();
    snake_.Grow();
  }
  EXPECT_EQ(game_model_.GetGameState(), GameOver);
}

TEST_F(GameModelTest, CheckCollisions_AppleEaten) {
  game_model_.SetGameState(Running);
  game_model_.UpdateGame();
  game_model_.HandleAppleEating();
  game_model_.UpdateGame();
  EXPECT_EQ(game_model_.GetScore(), 1);
}

TEST_F(GameModelTest, CheckWin) {
  game_model_.SetGameState(Running);
  for (auto i = 0; i < 201; ++i) {
    game_model_.HandleAppleEating();
    game_model_.IncrementScore();
  }
  game_model_.UpdateGame();
  EXPECT_EQ(game_model_.GetGameState(), Win);
}

class ScoreTest : public ::testing::Test {

protected:
  GameModel game_model_;
  void SetUp() override { std::remove("highscore.txt"); }
  void TearDown() override { std::remove("highscore.txt"); }
};

TEST_F(ScoreTest, InitialScore) {
  game_model_.ResetGame();
  EXPECT_EQ(game_model_.GetScore(), 0);
  EXPECT_EQ(game_model_.GetLevel(), 1);
  EXPECT_EQ(game_model_.GetSpeed(), BASE_SPEED_S);
}

TEST_F(ScoreTest, IncrementScore) {
  game_model_.IncrementScore();
  EXPECT_EQ(game_model_.GetScore(), 1);
  EXPECT_EQ(game_model_.GetHighScore(), 1);
}

TEST_F(ScoreTest, HighScoreUpdate) {
  game_model_.IncrementScore();
  EXPECT_EQ(game_model_.GetHighScore(), 1);
  game_model_.Reset();
  EXPECT_EQ(game_model_.GetHighScore(), 1);
  game_model_.IncrementScore();
  EXPECT_EQ(game_model_.GetHighScore(), 1);
  game_model_.IncrementScore();
  EXPECT_EQ(game_model_.GetHighScore(), 2);
}

TEST_F(ScoreTest, SaveAndLoadHighScore) {
  game_model_.IncrementScore();
  game_model_.SaveHighScore();
  EXPECT_EQ(game_model_.GetHighScore(), 1);

  game_model_.IncrementScore();
  game_model_.IncrementScore();
  game_model_.SaveHighScore();

  EXPECT_EQ(game_model_.GetHighScore(), 3);
}

class SnakeTest : public ::testing::Test {
protected:
  Snake snake;
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(SnakeTest, InitialPosition) {
  Position head = snake.GetHeadPosition();
  EXPECT_EQ(head.x, FIELD_WIDTH / 2);
  EXPECT_EQ(head.y, FIELD_HEIGHT / 2);
}

TEST_F(SnakeTest, MoveUp) {
  snake.ChangeDirection(Direction::up);
  snake.Move();
  Position head = snake.GetHeadPosition();
  EXPECT_EQ(head.y, (FIELD_HEIGHT / 2) - 1);
}

TEST_F(SnakeTest, MoveDown) {
  snake.ChangeDirection(Direction::up);
  snake.Move();
  snake.ChangeDirection(Direction::right);
  snake.Move();
  snake.ChangeDirection(Direction::down);
  snake.Move();
  snake.Move();
  Position head = snake.GetHeadPosition();
  EXPECT_EQ(head.y, (FIELD_HEIGHT / 2) + 1);
}

TEST_F(SnakeTest, MoveLeft) {
  snake.ChangeDirection(Direction::left);
  snake.Move();
  Position head = snake.GetHeadPosition();
  EXPECT_EQ(head.x, (FIELD_WIDTH / 2) - 1);
}

TEST_F(SnakeTest, MoveRight) {
  snake.ChangeDirection(Direction::right);
  snake.Move();
  Position head = snake.GetHeadPosition();
  EXPECT_EQ(head.x, (FIELD_WIDTH / 2) + 1);
}

TEST_F(SnakeTest, Grow) {
  size_t initial_size = snake.GetBody().size();
  snake.Grow();
  EXPECT_EQ(snake.GetBody().size(), initial_size + 1);
}

TEST_F(SnakeTest, ChangeDirection_Invalid) {
  snake.ChangeDirection(Direction::down);
  snake.Move();
  Position head = snake.GetHeadPosition();
  EXPECT_EQ(head.y, (FIELD_HEIGHT / 2) - 1);
}

TEST_F(SnakeTest, CheckSelfCollision_NoCollision) {
  EXPECT_FALSE(snake.CheckSelfCollision());
}

class TimerTest : public ::testing::Test {
protected:
  GameModel game_model_;
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(TimerTest, InitialState) { EXPECT_FALSE(game_model_.IsTimeToUpdate()); }

TEST_F(TimerTest, StartAndStop) {
  game_model_.Start();
  EXPECT_FALSE(game_model_.IsTimeToUpdate());
  game_model_.Stop();
  EXPECT_FALSE(game_model_.IsTimeToUpdate());
}

TEST_F(TimerTest, IsTimeToUpdate) {
  game_model_.SetInterval(100);
  game_model_.Start();
  EXPECT_FALSE(game_model_.IsTimeToUpdate());
}

} // namespace s21

class brick_game_tests : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST(brick_game_tests, GameInit) {
  GameInfo_t *game = game_init();
  ASSERT_NE(game, nullptr);
  ASSERT_NE(game->field, nullptr);
  ASSERT_NE(game->next_figure, nullptr);
  ASSERT_EQ(game->score, 0);
  ASSERT_EQ(game->level, 1);
  ASSERT_EQ(game->speed, 0);
  ASSERT_NE(game->figure, nullptr);
  ASSERT_EQ(game->status, Pause);
  ASSERT_EQ(game->action, IDLE);
  ASSERT_EQ(game->ticks_left, TICKS_START);
  free_game_init(game);
}

TEST(brick_game_tests, InitGameField) {
  int **field = init_game_field();
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      ASSERT_EQ(field[i][j], 0);
    }
  }
  free_game_field(field);
}

TEST(brick_game_tests, InitFigure) {
  Figure *figure = init_figure(FIGURE_START_X, FIGURE_START_Y);
  ASSERT_GE(figure->figure_num, 0);
  ASSERT_LT(figure->figure_num, FIGURES_COUNT);
  ASSERT_EQ(figure->x, FIGURE_START_X);
  ASSERT_EQ(figure->y, FIGURE_START_Y);
  free_figure(figure);
}

TEST(brick_game_tests, InitFigureValidInputs) {
  int figure_x = 5;
  int figure_y = 5;
  Figure *f = init_figure(figure_x, figure_y);

  ASSERT_NE(f, nullptr);
  ASSERT_EQ(f->x, figure_x);
  ASSERT_EQ(f->y, figure_y);
  ASSERT_NE(f->figure, nullptr);

  for (int i = 0; i < FIGURE_SIZE; i++) {
    ASSERT_NE(f->figure[i], nullptr);
  }

  free_figure(f);
}

TEST(brick_game_tests, InitFigureInvalidInputs) {
  int figure_x = -1;
  int figure_y = -1;
  Figure *f = init_figure(figure_x, figure_y);

  ASSERT_NE(f, nullptr);
  ASSERT_EQ(f->x, figure_x);
  ASSERT_EQ(f->y, figure_y);
  ASSERT_NE(f->figure, nullptr);

  for (int i = 0; i < FIGURE_SIZE; i++) {
    ASSERT_NE(f->figure[i], nullptr);
  }

  free_figure(f);
}

TEST(brick_game_tests, GetRandomFigure) {
  Figure *figure = init_figure(0, 0);
  get_random_figure(figure);

  ASSERT_GE(figure->figure_num, 0);
  ASSERT_LE(figure->figure_num, 6);

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
    for (int j = 0; j < FIGURE_SIZE; j++) {
      ASSERT_EQ(figure->figure[i][j], figures[figure->figure_num][i][j]);
    }
  }

  free_figure(figure);
}

TEST(brick_game_tests, PlaceFigureOnFieldValid) {
  GameInfo_t *game = game_init();

  place_figure_on_field(game);

  ASSERT_EQ(game->field[2][9], 0);
  clear_figure_from_field(game);
  ASSERT_EQ(game->field[2][9], 0);

  free_game_init(game);
}

TEST(brick_game_tests, CollisionWithinField) {
  GameInfo_t *game = game_init();

  ASSERT_EQ(collision(game), 0);

  free_game_init(game);
}

TEST(brick_game_tests, DropFilledLinesEmptyField) {
  GameInfo_t *game = game_init();

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 0;
    }
  }
  drop_filled_lines(0, game);
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      ASSERT_EQ(game->field[i][j], 0);
    }
  }
  free_game_init(game);
}

TEST(brick_game_tests, DropFilledLinesFilledField) {
  GameInfo_t *game = game_init();

  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 1;
    }
  }
  drop_filled_lines(FIELD_HEIGHT - 1, game);
  for (int i = 0; i < FIELD_HEIGHT - 1; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      ASSERT_EQ(game->field[i][j], 1);
    }
  }
  for (int j = 0; j < FIELD_WIDTH; j++) {
    ASSERT_EQ(game->field[FIELD_HEIGHT - 1][j], 1);
  }
  free_game_init(game);
}

TEST(brick_game_tests, EraseAndScoreOneLine) {
  GameInfo_t *game = game_init();

  for (int j = 0; j < FIELD_WIDTH; j++) {
    game->field[FIELD_HEIGHT - 1][j] = 1;
  }
  erase_and_score(game);
  ASSERT_EQ(game->score, 100);
  free_game_init(game);
}

TEST(brick_game_tests, EraseAndScoreTwoLines) {
  GameInfo_t *game = game_init();

  for (int i = FIELD_HEIGHT - 2; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 1;
    }
  }
  erase_and_score(game);
  ASSERT_EQ(game->score, 300);
  free_game_init(game);
}

TEST(brick_game_tests, EraseAndScoreThreeLines) {
  GameInfo_t *game = game_init();

  for (int i = FIELD_HEIGHT - 3; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 1;
    }
  }
  erase_and_score(game);
  ASSERT_EQ(game->score, 700);
  free_game_init(game);
}

TEST(brick_game_tests, EraseAndScoreFourLines) {
  GameInfo_t *game = game_init();

  for (int i = FIELD_HEIGHT - 4; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field[i][j] = 1;
    }
  }
  erase_and_score(game);
  ASSERT_EQ(game->score, 1500);
  free_game_init(game);
}

TEST(brick_game_tests, CheckTicksNoCollision) {
  GameInfo_t *game = game_init();

  game->ticks_left = 0;
  game->status = Start;

  check_ticks(game);

  ASSERT_EQ(game->ticks_left, TICKS_START);
  ASSERT_EQ(game->status, Start);
  free_game_init(game);
}

TEST(brick_game_tests, CheckTicksTicksLeftPositive) {
  GameInfo_t *game = game_init();

  game->ticks_left = 5;
  game->status = Start;

  check_ticks(game);

  ASSERT_EQ(game->ticks_left, 5);
  ASSERT_EQ(game->status, Start);
  free_game_init(game);
}

TEST(brick_game_tests, CalculateLevel) {
  GameInfo_t *game = game_init();

  game->score = 1200;
  game->level = 1;

  calculate_level(game);

  ASSERT_EQ(game->level, 3);
  free_game_init(game);
}

TEST(brick_game_tests, CalculateLevelMax) {
  GameInfo_t *game = game_init();

  game->score = 6000;
  game->level = 1;

  calculate_level(game);

  ASSERT_EQ(game->level, 10);
  free_game_init(game);
}

TEST(brick_game_tests, CalculateSpeed) {
  GameInfo_t *game = game_init();

  game->level = 2;

  calculate_speed(game);

  ASSERT_EQ(game->speed, 2 * BASE_SPEED);
  free_game_init(game);
}

TEST(brick_game_tests, UpdateMaxScore) {
  GameInfo_t *game = game_init();

  game->score = 100;
  game->high_score = 50;

  update_max_score(game);
  ASSERT_EQ(game->high_score, 100);

  game->score = 40;
  update_max_score(game);
  ASSERT_EQ(game->high_score, 100);
  free_game_init(game);
}

TEST(brick_game_tests, SaveMaxScore) {
  GameInfo_t *game = game_init();
  plant_check_collision_and_score(game);

  game->high_score = 200;

  save_max_score(game);

  FILE *file = fopen("max_score.txt", "r");
  ASSERT_NE(file, nullptr) << "File not found";

  int max_score;
  fscanf(file, "%d", &max_score);
  fclose(file);

  ASSERT_EQ(max_score, 200);
  free_game_init(game);
}

TEST(brick_game_tests, CalculateGameUp) {
  GameInfo_t *game = game_init();
  game->action = Up;
  calculate_game(game);
  ASSERT_EQ(game->figure->x, 3);
  free_game_init(game);
}

TEST(brick_game_tests, CalculateGameLeft) {
  GameInfo_t *game = game_init();

  game->action = Left;
  calculate_game(game);
  ASSERT_EQ(game->figure->x, 2);
  free_game_init(game);
}

TEST(brick_game_tests, CalculateGameRight) {
  GameInfo_t *game = game_init();

  game->action = Right;
  calculate_game(game);
  ASSERT_EQ(game->figure->x, 4);
  free_game_init(game);
}

TEST(brick_game_tests, CalculateGameDown) {
  GameInfo_t *game = game_init();

  game->action = Down;
  calculate_game(game);
  ASSERT_EQ(game->figure->y, 1);
  free_game_init(game);
}

TEST(brick_game_tests, CalculateGamePause) {
  GameInfo_t *game = game_init();

  game->action = Pause;
  calculate_game(game);
  ASSERT_EQ(game->status, Pause);

  free_game_init(game);
}

TEST(brick_game_tests, CalculateGameTerminate) {
  GameInfo_t *game = game_init();

  game->action = Terminate;
  calculate_game(game);
  ASSERT_EQ(game->status, Terminate);
  free_game_init(game);
}

TEST(brick_game_tests, CalculateGameStart) {
  GameInfo_t *game = game_init();

  game->action = Start;
  calculate_game(game);
  ASSERT_EQ(game->status, Start);

  free_game_init(game);
}

TEST(brick_game_tests, CalculateGameReset) {
  GameInfo_t *game = game_init();

  game->status = GAMEOVER;
  calculate_game(game);
  get_user_action(game, '\n');
  calculate_game(game);
  ASSERT_EQ(game->status, RESET);

  free_game_init(game);
}

TEST(brick_game_tests, CalculateAction) {
  GameInfo_t *game = game_init();

  game->status = Start;
  calculate_game(game);
  get_user_action(game, 'x');
  calculate_game(game);
  get_user_action(game, 'X');
  calculate_game(game);
  game->status = Action;
  calculate_game(game);
  ASSERT_EQ(game->status, Action);

  free_game_init(game);
}

TEST(brick_game_tests, GetUserActionUp) {
  GameInfo_t *game = game_init();

  game->status = Start;
  get_user_action(game, KEY_UP);
  ASSERT_EQ(game->action, Up);
  free_game_init(game);
}

TEST(brick_game_tests, GetUserActionLeft) {
  GameInfo_t *game = game_init();

  game->status = Start;
  get_user_action(game, KEY_LEFT);
  ASSERT_EQ(game->action, Left);
  free_game_init(game);
}

TEST(brick_game_tests, GetUserActionRight) {
  GameInfo_t *game = game_init();

  game->status = Start;
  get_user_action(game, KEY_RIGHT);
  ASSERT_EQ(game->action, Right);
  free_game_init(game);
}

TEST(brick_game_tests, GetUserActionDown) {
  GameInfo_t *game = game_init();

  game->status = Start;
  get_user_action(game, KEY_DOWN);
  ASSERT_EQ(game->action, Down);
  free_game_init(game);
}

TEST(brick_game_tests, GetUserActionAction) {
  GameInfo_t *game = game_init();

  game->status = Start;
  get_user_action(game, 'x');
  ASSERT_EQ(game->action, Action);
  get_user_action(game, 'X');
  ASSERT_EQ(game->action, Action);
  free_game_init(game);
}

TEST(brick_game_tests, GetUserActionPause) {
  GameInfo_t *game = game_init();

  game->status = Start;
  get_user_action(game, 'p');
  ASSERT_EQ(game->action, Pause);
  get_user_action(game, 'P');
  ASSERT_EQ(game->action, Pause);
  free_game_init(game);
}

TEST(brick_game_tests, GetUserActionTerminate) {
  GameInfo_t *game = game_init();

  game->status = Start;
  get_user_action(game, 'q');
  ASSERT_EQ(game->action, Terminate);
  get_user_action(game, 'Q');
  ASSERT_EQ(game->action, Terminate);
  free_game_init(game);
}

TEST(brick_game_tests, GetUserActionStart) {
  GameInfo_t *game = game_init();

  game->status = Start;
  get_user_action(game, '\n');
  ASSERT_EQ(game->action, Start);
  free_game_init(game);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
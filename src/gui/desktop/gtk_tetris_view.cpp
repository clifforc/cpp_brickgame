#include "inc/game_view.h"

namespace s21 {
bool GameWindow::key_handling_tetris(guint keyval, guint keycode,
                                     Gdk::ModifierType state) {
  switch (keyval) {
  case GDK_KEY_Left:
    tetris_game_info_->action = Left;
    break;
  case GDK_KEY_Right:
    tetris_game_info_->action = Right;
    break;
  case GDK_KEY_Up:
    tetris_game_info_->action = Up;
    break;
  case GDK_KEY_Down:
    tetris_game_info_->action = Down;
    break;
  case GDK_KEY_x:
  case GDK_KEY_X:
    tetris_game_info_->action = Action;
    break;
  case GDK_KEY_p:
  case GDK_KEY_P:
    tetris_game_info_->action = Pause;
    break;
  case GDK_KEY_q:
  case GDK_KEY_Q:
    tetris_game_info_->action = Terminate;
    break;
  case GDK_KEY_Return:
    tetris_game_info_->action = Start;
    break;
  default:
    tetris_game_info_->action = IDLE;
    break;
  }
  return true;
}

bool GameWindow::on_timeout_tetris() {
  if (tetris_game_info_ == nullptr) {
    return true;
  }

  if (tetris_game_info_->status == GAMEOVER) {
    free_tetris_game();
    initialize_tetris_game();
  }

  calculate_game(tetris_game_info_);

  if (tetris_game_info_->status == Terminate) {
    hide();
    free_tetris_game();
    return false;
  }

  if (tetris_game_info_->status != Pause && tetris_game_info_->status != GAMEOVER) {
    level_ = tetris_game_info_->level;
    score_ = tetris_game_info_->score;
    max_score_ = tetris_game_info_->high_score;

    score_label_.set_text("LEVEL: " + std::to_string(level_));
    level_label_.set_text("SCORE: " + std::to_string(score_));
    max_score_label_.set_text("MAX SCORE: " + std::to_string(max_score_));

    int temp_field[FIELD_HEIGHT][FIELD_WIDTH];
    for (int row = 0; row < FIELD_HEIGHT; ++row) {
      for (int col = 0; col < FIELD_WIDTH; ++col) {
        temp_field[row][col] = tetris_game_info_->field[row][col];
      }
    }

    for (int i = 0; i < FIGURE_SIZE; i++) {
      for (int j = 0; j < FIGURE_SIZE; j++) {
        if (tetris_game_info_->figure->figure[i][j] != 0) {
          int field_x = tetris_game_info_->figure->x + j;
          int field_y = tetris_game_info_->figure->y + i - 2;

          if (field_x >= 0 && field_x < FIELD_WIDTH && field_y >= 0 && field_y < FIELD_HEIGHT) {
            temp_field[field_y][field_x] = tetris_game_info_->figure->figure[i][j];
          }
        }
      }
    }

    for (int row = 0; row < FIELD_HEIGHT; ++row) {
      for (int col = 0; col < FIELD_WIDTH; ++col) {
        int cell_value = temp_field[row][col];
        std::string color = get_cell_color(cell_value);
        set_cell_color(row, col, color);
      }
    }

    for (int row = 0; row < FIGURE_SIZE; ++row) {
      for (int col = 0; col < FIGURE_SIZE; ++col) {
        int cell_value = tetris_game_info_->next_figure->figure[row][col];
        std::string color = get_cell_color(cell_value);
        set_next_figure_cell_color(row, col, color);
      }
    }
  }

  if (tetris_game_info_->status == Pause) {
    pause_message_label_.set_visible(true);
    game_over_label_.set_visible(false);
    win_label_.set_visible(false);
  } else if (tetris_game_info_->status == GAMEOVER) {
    pause_message_label_.set_visible(false);
    game_over_label_.set_visible(true);
    win_label_.set_visible(false);
  } else if (tetris_game_info_->status == Start) {
    pause_message_label_.set_visible(false);
    game_over_label_.set_visible(false);
    win_label_.set_visible(false);
  }

  return true;
}

void GameWindow::initialize_tetris_game() {
  if (tetris_game_info_ == nullptr) {
    tetris_game_info_ = game_init();
  }
}
void GameWindow::free_tetris_game() {
  if (tetris_game_info_) {
    free_game_init(tetris_game_info_);
    tetris_game_info_ = nullptr;
  }
}
} // namespace s21

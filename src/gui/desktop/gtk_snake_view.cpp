#include "inc/game_view.h"

namespace s21{
bool GameWindow::key_handling_snake(guint keyval, guint keycode,
                              Gdk::ModifierType state) {
  switch (keyval) {
  case GDK_KEY_Left:
    game_controller_->userInput(Left, false);
    break;
  case GDK_KEY_Right:
    game_controller_->userInput(Right, false);
    break;
  case GDK_KEY_Up:
    game_controller_->userInput(Up, false);
    break;
  case GDK_KEY_Down:
    game_controller_->userInput(Down, false);
    break;
  case GDK_KEY_x:
  case GDK_KEY_X:
    game_controller_->userInput(Action, true);
    break;
  case GDK_KEY_p:
  case GDK_KEY_P:
    game_controller_->userInput(Pause, false);
    break;
  case GDK_KEY_q:
  case GDK_KEY_Q:
    game_controller_->userInput(Terminate, false);
    break;
  case GDK_KEY_Return:
    game_controller_->userInput(Start, false);
    break;
  default:
    break;
  }
  return true;
}

bool GameWindow::key_release_handling_snake(guint keyval, guint keycode,
                                            Gdk::ModifierType state) {
  if (keyval == GDK_KEY_x || keyval == GDK_KEY_X) {
    game_controller_->userInput(Action, false);
    return true;
  }
  return false;
}

bool GameWindow::on_timeout_snake() {
  game_info_ = game_model_->UpdateCurrentState();
  GameState state = game_model_->GetGameState();

  if (state == Exit) {
    hide();
    return false;
  }

  if (state != Paused && state != GameOver && state != Win) {
    level_ = game_info_.level;
    score_ = game_info_.score;
    max_score_ = game_info_.high_score;

    score_label_.set_text("LEVEL: " + std::to_string(level_));
    level_label_.set_text("SCORE: " + std::to_string(score_));
    max_score_label_.set_text("MAX SCORE: " + std::to_string(max_score_));

    for (int row = 0; row < FIELD_HEIGHT; ++row) {
      for (int col = 0; col < FIELD_WIDTH; ++col) {
        int cell_value = game_info_.field[row][col];
        std::string color = get_cell_color(cell_value);
        set_cell_color(row, col, color);
      }
    }
  }

  if (state == Paused) {
    pause_message_label_.set_visible(true);
    game_over_label_.set_visible(false);
    win_label_.set_visible(false);
  } else if (state == GameOver) {
    pause_message_label_.set_visible(false);
    game_over_label_.set_visible(true);
    win_label_.set_visible(false);
  } else if (state == Win) {
    pause_message_label_.set_visible(false);
    game_over_label_.set_visible(false);
    win_label_.set_visible(true);
  } else if (state == Running) {
    pause_message_label_.set_visible(false);
    game_over_label_.set_visible(false);
    win_label_.set_visible(false);
  }

  return true;
}
}

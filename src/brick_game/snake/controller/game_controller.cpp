#include "inc/game_controller.h"

namespace s21 {

GameController::GameController(GameModel *game_model) : game_model_(game_model) {}

void GameController::userInput(UserAction_t action, bool hold) {
  (void)hold;
  switch (action) {
  case Start:
    game_model_->SetGameState(Running);
    break;
  case Pause:
    if (game_model_->GetGameState() != GameOver &&
        game_model_->GetGameState() != Win){
      game_model_->SetGameState(Paused);
    }
    break;
  case Terminate:
    game_model_->SetGameState(Exit);
    break;
  case Left:
    game_model_->SetSnakeDirection(Direction::left);
    break;
  case Right:
    game_model_->SetSnakeDirection(Direction::right);
    break;
  case Up:
    game_model_->SetSnakeDirection(Direction::up);
    break;
  case Down:
    game_model_->SetSnakeDirection(Direction::down);
    break;
  case Action:
    game_model_->SetSpeedUp(hold);;
    break;
  default:
    break;
  }
}

} // namespace s21

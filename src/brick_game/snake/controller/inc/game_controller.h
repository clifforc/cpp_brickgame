#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include <memory>

#include "../../model/inc/game_model.h"

namespace s21 {

class GameController {
 private:
  GameModel *game_model_;

 public:
  GameController(GameModel *game_model);

  void userInput(UserAction_t action, bool hold);
};

}  // namespace s21

#endif

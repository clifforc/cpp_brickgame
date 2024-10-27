#include "inc/apple.h"


namespace s21 {

Apple::Apple()
    : position_(0, 0),
      generator_(std::mt19937(std::random_device{}())) {}

void Apple::SpawnApple(const std::vector<Position> &occupied_position) {
  std::uniform_int_distribution<int> dist_x(0, FIELD_WIDTH - 1);
  std::uniform_int_distribution<int> dist_y(0, FIELD_HEIGHT - 1);

  while (true) {
    int x = dist_x(generator_);
    int y = dist_y(generator_);
    position_ = Position(x, y);

    if (!std::any_of(occupied_position.begin(), occupied_position.end(),
                     [&](const Position& pos) { return position_ == pos; })) {
      break;
    }
  }
}

const Position& Apple::GetPosition() const {
  return position_;
}

} // namespace s21

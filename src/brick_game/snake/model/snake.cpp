#include "inc/snake.h"

namespace s21 {

Snake::Snake() {
  int start_x = FIELD_WIDTH / 2;
  int start_y = FIELD_HEIGHT / 2;
  for (int i = 0; i < 4; ++i) {
    body_.emplace_back(start_x, start_y + i);
  }
  current_direction_ = Direction::up;
  next_direction_ = Direction::up;
}

void Snake::Move() {
  current_direction_ = next_direction_;
  Position head = GetHeadPosition();
  switch (current_direction_) {
  case Direction::up:
    head.y -= 1;
    break;
  case Direction::down:
    head.y += 1;
    break;
  case Direction::left:
    head.x -= 1;
    break;
  case Direction::right:
    head.x += 1;
    break;
  }
  body_.emplace_front(head.x, head.y);
  body_.pop_back();
}

void Snake::Grow() {
  Position tail = body_.back().position;
  body_.emplace_back(tail.x, tail.y);
}

void Snake::ChangeDirection(Direction new_dir) {
  if ((current_direction_ == Direction::up && new_dir != Direction::down) ||
      (current_direction_ == Direction::down && new_dir != Direction::up) ||
      (current_direction_ == Direction::left && new_dir != Direction::right) ||
      (current_direction_ == Direction::right && new_dir != Direction::left)) {
    next_direction_ = new_dir;
  }
}

bool Snake::CheckSelfCollision() const {
  const Position &head = GetHeadPosition();
  for (size_t i = 1; i < body_.size(); ++i) {
    if (head == body_[i].position) {
      return true;
    }
  }
  return false;
}

const Position &Snake::GetHeadPosition() const {
  return body_.front().position;
}

const std::deque<SnakeSegment> &Snake::GetBody() const { return body_; }

const std::vector<Position> Snake::GetOccupiedPositon() const {
  std::vector<Position> positions;
  positions.reserve(body_.size());
for(const auto& segment : body_){
    positions.push_back(segment.position);
  }
  return positions;
}

} // namespace s21

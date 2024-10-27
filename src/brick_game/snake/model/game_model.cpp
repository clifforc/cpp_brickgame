#include "inc/game_model.h"

namespace s21 {

GameModel::GameModel()
    : snake_(), apple_(), score_(0), high_score_(0), level_(1),
      speed_(BASE_SPEED_S), interval_(BASE_SPEED_S),
      original_interval_(BASE_SPEED_S), running_(false),
      speed_up_active_(false), state_(Paused) {
  apple_.SpawnApple(snake_.GetOccupiedPositon());
  LoadHighScore();
  SetGameState(Paused);
  InitializeField();
}

GameModel::~GameModel() { ClearField(); }

GameInfo_t GameModel::UpdateCurrentState() {
  if (state_ == Running && IsTimeToUpdate()) {
    UpdateGame();
  }

  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    std::memset(field_[i], 0, FIELD_WIDTH * sizeof(int));
  }

  for (const auto &segment : snake_.GetBody()) {
    if (CheckIsOnField(segment.position)) {
      field_[segment.position.y][segment.position.x] = 3;
    }
  }

  Position apple_pos = apple_.GetPosition();
  field_[apple_pos.y][apple_pos.x] = 2;

  GameInfo_t game_info;
  game_info.field = field_.get();
  game_info.score = score_;
  game_info.high_score = high_score_;
  game_info.level = level_;
  game_info.speed = speed_;
  game_info.pause = state_ == Paused ? 1 : 0;

  return game_info;
}

GameState GameModel::GetGameState() const { return state_; }

void GameModel::InitializeField() {
  field_ = std::make_unique<int *[]>(FIELD_HEIGHT);
  for (int i = 0; i < FIELD_HEIGHT; ++i) {
    field_[i] = new int[FIELD_WIDTH];
    std::memset(field_[i], 0, FIELD_WIDTH * sizeof(int));
  }
}

void GameModel::ClearField() {
  if (field_) {
    for (int i = 0; i < FIELD_HEIGHT; ++i) {
      delete[] field_[i];
    }
    field_.reset();
  }
}

void GameModel::ResetGame() {
  snake_ = Snake();
  apple_.SpawnApple(snake_.GetOccupiedPositon());
  Reset();
  speed_ = BASE_SPEED_S;
  interval_ = BASE_SPEED_S;
  running_ = false;
  speed_up_active_ = false;
  state_ = GameOver;
  ClearField();
  InitializeField();
}

void GameModel::SetGameState(GameState state) {
  state_ = state;
  if (state_ == Running) {
    Start();
  } else if (state_ == GameOver) {
    ResetGame();
  } else {
    Stop();
  }
}

void GameModel::SetSnakeDirection(Direction direction) {
  snake_.ChangeDirection(direction);
}

void GameModel::SetSpeedUp(bool hold) {
  if (hold && !speed_up_active_) {
    speed_up_active_ = true;
    interval_ = MAX_SPEED;
  } else if (!hold && speed_up_active_) {
    speed_up_active_ = false;
    interval_ = original_interval_;
  }
}

void GameModel::CheckCollisions() {
  const Position &head = snake_.GetHeadPosition();
  if (!CheckIsOnField(head) || snake_.CheckSelfCollision()) {
    HandleWinLoose(GameOver);
  } else if (head == apple_.GetPosition()) {
    HandleAppleEating();
  }
}

void GameModel::HandleWinLoose(GameState state) {
  Stop();
  SetGameState(state);
  UpdateHighScore();
  SaveHighScore();
}

void GameModel::HandleAppleEating() {
  snake_.Grow();
  IncrementScore();
  apple_.SpawnApple(snake_.GetOccupiedPositon());
  if (snake_.GetBody().size() >= 200) {
    HandleWinLoose(Win);
  }
  if (CheckForLevelUp()) {
    SetInterval(GetSpeed());
  }
}

void GameModel::UpdateGame() {
  snake_.Move();
  CheckCollisions();
}

bool GameModel::CheckIsOnField(Position position) const {
  return position.x >= 0 && position.x < FIELD_WIDTH && position.y >= 0 &&
         position.y < FIELD_HEIGHT;
}

void GameModel::IncrementScore() {
  ++score_;
  UpdateHighScore();
}

int GameModel::GetScore() const { return score_; }
int GameModel::GetLevel() const { return level_; }
int GameModel::GetSpeed() const { return speed_; }
int GameModel::GetHighScore() const { return high_score_; }

void GameModel::Reset() {
  score_ = 0;
  level_ = 1;
  speed_ = BASE_SPEED_S;
}

bool GameModel::CheckForLevelUp() {
  if (score_ % POINTS_PER_LEVEL == 0 && level_ < MAX_LEVEL) {
    ++level_;
    UpdateSpeed();
    return true;
  }
  return false;
}

void GameModel::UpdateSpeed() { speed_ = BASE_SPEED_S - (level_ - 1) * 40; }

void GameModel::UpdateHighScore() {
  high_score_ = score_ > high_score_ ? score_ : high_score_;
}

void GameModel::LoadHighScore() {
  std::ifstream file("highscore.txt");
  if (file.is_open()) {
    file >> high_score_;
    file.close();
  } else {
    high_score_ = 0;
  }
}

void GameModel::SaveHighScore() {
  std::ofstream file("highscore.txt");
  if (file.is_open()) {
    file << high_score_;
    file.close();
  }
}

void GameModel::Start() {
  running_ = true;
  last_update_time_ = std::chrono::steady_clock::now();
}

void GameModel::Stop() { running_ = false; }
void GameModel::SetInterval(int msec) {
  original_interval_ = msec;
  if (!speed_up_active_) {
    interval_ = original_interval_;
  }
}

bool GameModel::IsTimeToUpdate() {
  if (!running_)
    return false;
  auto now = std::chrono::steady_clock::now();
  auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
      now - last_update_time_);
  if (elapsed.count() >= int(interval_)) {
    last_update_time_ = now;
    return true;
  }
  return false;
}

} // namespace s21

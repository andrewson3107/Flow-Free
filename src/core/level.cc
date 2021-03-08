#include <core/level.h>

namespace Flow {

Level::Level()
    : initial_({0}),
      solution_({0}),
      dimension_(0),
      is_complete_(false),
      best_score_(0) {
}

Level::Level(const Layout& initial, const Layout& complete,
             const size_t& dimension)
    : initial_(initial),
      solution_(complete),
      dimension_(dimension),
      is_complete_(false),
      best_score_(0) {
}

const Layout& Level::GetInitialState() const {
  return initial_;
}

const Layout& Level::GetSolution() const {
  return solution_;
}

size_t Level::GetDimension() const {
  return dimension_;
}

bool Level::IsComplete() const {
  return is_complete_;
}

void Level::SetIsComplete() {
  is_complete_ = true;
}

void Level::SetBestScore(size_t score) {
  best_score_ = score;
}

size_t Level::GetBestScore() const {
  return best_score_;
}
}  // namespace Flow
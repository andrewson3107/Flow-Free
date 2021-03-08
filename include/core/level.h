#pragma once

#include <deque>
#include <vector>

namespace Flow {
typedef std::vector<std::vector<char>> Layout;

class Level {
 public:
  Level();
  Level(const Layout& initial, const Layout& complete,
        const size_t& dimension);

  const Layout& GetInitialState() const;
  const Layout& GetSolution() const;
  size_t GetDimension() const;
  bool IsComplete() const;
  void SetIsComplete();
  void SetBestScore(size_t score);
  size_t GetBestScore() const;

 private:
  Layout solution_;
  Layout initial_;
  size_t dimension_;
  bool is_complete_;
  size_t best_score_;
};
}  // namespace Flow
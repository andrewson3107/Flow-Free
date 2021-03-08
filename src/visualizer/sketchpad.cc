#include <cinder/Color.h>
#include <cinder/gl/gl.h>
#include <visualizer/flow_app.h>
#include <visualizer/sketchpad.h>

namespace Flow {

namespace visualizer {

Sketchpad::Sketchpad(const glm::vec2& top_left_corner, float side_length)
    : top_left_corner_(top_left_corner),
      side_length_(side_length),
      bottom_left_corner_(top_left_corner + glm::vec2(0, side_length)),
      num_moves_(0) {
  ReadLevels(kLevelFilePath);
}

void Sketchpad::Draw() const {
  // Black rectangle with subtle white outline
  ci::gl::color(ci::Color("black"));
  ci::gl::drawSolidRect(
      ci::Rectf(top_left_corner_,
                top_left_corner_ + glm::vec2(side_length_, side_length_)));

  ci::gl::color(255, 255, 255);  // White
  ci::gl::drawStrokedRect(
      ci::Rectf(top_left_corner_,
                top_left_corner_ + glm::vec2(side_length_, side_length_)),
      1);
}

void Sketchpad::DrawLevel(size_t level_number) {
  current_level_ = levels_[level_number];

  if (user_layout_.empty()) {
    user_layout_ = current_level_.GetInitialState();
  }

  DrawGrid();
  DrawPaths();
  DrawCircles();
  DrawStats();

  if (user_layout_ == current_level_.GetSolution()) {
    OnComplete(level_number);
  }
}

void Sketchpad::DrawGrid() const {
  ci::gl::color(1, 1, 1);  // White
  float grid_box_size = side_length_ / current_level_.GetDimension();

  for (size_t index = 1; index < current_level_.GetDimension(); index++) {
    ci::gl::drawLine(top_left_corner_ + glm::vec2(index * grid_box_size, 0),
                     bottom_left_corner_ + glm::vec2(index * grid_box_size, 0));

    ci::gl::drawLine(
        top_left_corner_ + glm::vec2(0, index * grid_box_size),
        top_left_corner_ + glm::vec2(side_length_, index * grid_box_size));
  }
}

void Sketchpad::DrawCircles() const {
  float grid_box_size = side_length_ / current_level_.GetDimension();

  for (size_t row = 0; row < current_level_.GetDimension(); row++) {
    for (size_t col = 0; col < current_level_.GetDimension(); col++) {

      // Circles should be drawn if the corresponding layout character is not
      // blank.
      if (current_level_.GetInitialState()[row][col] != kBlankChar) {
        // - '1' to convert from char to int and also to avoid zero index errors
        size_t circle_number = current_level_.GetInitialState()[row][col] - '1';

        SetColor(kColors[circle_number]);

        ci::gl::drawSolidCircle(
            top_left_corner_ +
                glm::vec2(col * grid_box_size + (grid_box_size / 2),
                          row * grid_box_size + (grid_box_size / 2)),
            grid_box_size / 4, 1000);
      }
    }
  }
}

void Sketchpad::DrawPaths() const {
  size_t dimension = current_level_.GetDimension();
  float grid_box_size = side_length_ / current_level_.GetDimension();

  for (size_t row = 0; row < dimension; row++) {
    for (size_t col = 0; col < dimension; col++) {

      // Paths should only be drawn on non blank boxes
      if (user_layout_[row][col] != kBlankChar) {

        // - '1' to convert to int and also to avoid zero index errors
        size_t circle_number = user_layout_[row][col] - '1';

        SetColor(kColors[circle_number]);

        ci::gl::drawSolidCircle(
            top_left_corner_ +
                glm::vec2(col * grid_box_size + (grid_box_size / 2),
                          row * grid_box_size + (grid_box_size / 2)),
            grid_box_size / 8, 1000);

        // Checks each surrounding grid. If two adjacent tiles are the same,
        // draws a path between them.

        if (col + 1 < dimension &&
            user_layout_[row][col] == user_layout_[row][col + 1]) {
          ci::gl::drawSolidRect(ci::Rectf(
              top_left_corner_ +
                  glm::vec2(col * grid_box_size + (grid_box_size / 2),
                            row * grid_box_size + (grid_box_size / 2) -
                                (grid_box_size / 8)),
              top_left_corner_ +
                  glm::vec2((col + 1) * grid_box_size,
                            row * grid_box_size + (grid_box_size / 2) +
                                (grid_box_size / 8))));
        }

        if (col > 0 && user_layout_[row][col] == user_layout_[row][col - 1]) {
          ci::gl::drawSolidRect(ci::Rectf(
              top_left_corner_ +
                  glm::vec2(col * grid_box_size + (grid_box_size / 2),
                            row * grid_box_size + (grid_box_size / 2) -
                                (grid_box_size / 8)),
              top_left_corner_ +
                  glm::vec2((col)*grid_box_size, row * grid_box_size +
                                                     (grid_box_size / 2) +
                                                     (grid_box_size / 8))));
        }

        if (row + 1 < dimension &&
            user_layout_[row][col] == user_layout_[row + 1][col]) {
          ci::gl::drawSolidRect(ci::Rectf(
              top_left_corner_ +
                  glm::vec2(col * grid_box_size + (grid_box_size / 2) -
                                (grid_box_size / 8),
                            row * grid_box_size + (grid_box_size / 2)),
              top_left_corner_ +
                  glm::vec2(col * grid_box_size + (grid_box_size / 2) +
                                (grid_box_size / 8),
                            (row + 1) * grid_box_size)));
        }

        if (row > 0 && user_layout_[row][col] == user_layout_[row - 1][col]) {
          ci::gl::drawSolidRect(ci::Rectf(
              top_left_corner_ +
                  glm::vec2(col * grid_box_size + (grid_box_size / 2) -
                                (grid_box_size / 8),
                            row * grid_box_size + (grid_box_size / 2)),
              top_left_corner_ +
                  glm::vec2(col * grid_box_size + (grid_box_size / 2) +
                                (grid_box_size / 8),
                            (row)*grid_box_size)));
        }
      }
    }
  }
}

void Sketchpad::DrawStats() const {
  // First calculates completion percentage by counting number of blank tiles in
  // initial layout vs user layout
  size_t count = 0;
  size_t total = 0;

  for (size_t row = 0; row < current_level_.GetDimension(); row++) {
    for (size_t col = 0; col < current_level_.GetDimension(); col++) {
      if (current_level_.GetInitialState()[row][col] == kBlankChar) {
        total++;
        if (user_layout_[row][col] != kBlankChar) {
          count++;
        }
      }
    }
  }

  size_t percent = (size_t)((double)count / total * 100);

  // num_moves and best score are already tracked
  ci::gl::drawStringCentered(
      "Pipe: " + std::to_string(percent) + "%",
      top_left_corner_ +
          glm::vec2(side_length_ - kStatMarginX, side_length_ + kStatMarginY),
      ci::Color("Black"), ci::Font("Times New Roman", kStatText));

  ci::gl::drawStringCentered(
      "Moves: " + std::to_string(num_moves_),
      top_left_corner_ + glm::vec2(side_length_ / 2 - kStatMarginX,
                                   side_length_ + kStatMarginY),
      ci::Color("Black"), ci::Font("Times New Roman", kStatText));

  ci::gl::drawStringCentered(
      "Best: " + std::to_string(current_level_.GetBestScore()),
      top_left_corner_ + glm::vec2(side_length_ / 2 + kStatMarginX,
                                   side_length_ + kStatMarginY),
      ci::Color("Black"), ci::Font("Times New Roman", kStatText));
}

void Sketchpad::DrawLevelSelect() const {
  // If the number of levels is not a perfect square, it will be incremented
  // until it is. This is so that an even grid can be drawn. If there are 12
  // levels, the level select screen will display 16 with 4 being unnumbered

  size_t num_levels = levels_.size();
  while (std::sqrt(num_levels) - std::floor(std::sqrt(num_levels)) != 0) {
    num_levels++;
  }
  double num_levels_per_side = std::sqrt(num_levels);

  ci::gl::color(1, 1, 1);
  float grid_box_size = side_length_ / (float)num_levels_per_side;

  // Draws a grid similar to the DrawGrid method
  size_t level_number = 1; // Used for tracking what number should be drawn
  for (size_t row = 0; row < num_levels_per_side; row++) {
    ci::gl::drawLine(top_left_corner_ + glm::vec2(row * grid_box_size, 0),
                     bottom_left_corner_ + glm::vec2(row * grid_box_size, 0));

    ci::gl::drawLine(
        top_left_corner_ + glm::vec2(0, row * grid_box_size),
        top_left_corner_ + glm::vec2(side_length_, row * grid_box_size));

    for (size_t col = 0; col < num_levels_per_side; col++) {

      // If the level is complete, an additional star is drawn.
      if (level_number < levels_.size() &&
          levels_[level_number - 1].IsComplete()) {
        ci::gl::drawStringCentered(
            kCompleteChar,
            top_left_corner_ +
                glm::vec2(col * grid_box_size + (grid_box_size / 2),
                          row * grid_box_size + (grid_box_size / 3)),
            "Blue", ci::Font(kFont, kStarText));
      }

      // The level number is also drawn on top.
      ci::gl::drawStringCentered(
          std::to_string(level_number),
          top_left_corner_ +
              glm::vec2(col * grid_box_size + (grid_box_size / 2),
                        row * grid_box_size + (grid_box_size / 3)),
          "White", ci::Font(kFont, kLevelNumText));

      level_number++;
    }
  }
}

void Sketchpad::ReadLevels(const std::string& file_path) {
  std::string line;
  std::ifstream ifs(file_path);

  // A 2d vector representing an n x n level.
  Layout initial_state;
  Layout complete_state;

  while (getline(ifs, line)) {

    // Takes the next line in the file and stores it in a vector of chars.
    std::vector<char> ints_in_line(line.begin(), line.end());

    // If the initial state layout is not symmetrical (is 2x3 or 1x3),
    // the vector of chars above is appended. Allows for levels of any size to
    // be read.
    if (initial_state.size() < line.length()) {
      initial_state.push_back(ints_in_line);
    }

    // After the initial state layout is populated, the next layout will be the
    // complete state.
    else if (complete_state.size() < line.length()) {
      complete_state.push_back(ints_in_line);

      // When the two layouts are the same size, the entire level has been read.
      // Both are added to levels then cleared.
      if (initial_state.size() == complete_state.size()) {
        levels_.push_back(Level(initial_state, complete_state, line.length()));
        initial_state.clear();
        complete_state.clear();
      }
    }
  }
}

void Sketchpad::HandleBrush(const glm::vec2& mouse_coords) {

  // Takes the mouse coordinates and divides them by the size of a grid box
  // This converts them to numbers that can be used as indices.
  // Ex: an x-coordinate of 550 with a grid box size of 100 means the mouse is
  // in the 5th zero-indexed box.
  float grid_box_size = side_length_ / current_level_.GetDimension();
  glm::vec2 mouse_pos = mouse_coords - top_left_corner_;
  size_t col = (size_t)std::floor(mouse_pos.x / grid_box_size);
  size_t row = (size_t)std::floor(mouse_pos.y / grid_box_size);

  // Changes the character to the current pipe number
  user_layout_[row][col] = pipe_number_;

  // The initial circles should not be able to be modified, thus set them back to
  // initial values.
  for (size_t row = 0; row < current_level_.GetDimension(); row++) {
    for (size_t col = 0; col < current_level_.GetDimension(); col++) {
      if (current_level_.GetInitialState()[row][col] != kBlankChar) {
        user_layout_[row][col] = current_level_.GetInitialState()[row][col];
      }
    }
  }
}

size_t Sketchpad::SetCurrentLevel(const glm::vec2& mouse_coords) {
  size_t temp = levels_.size();

  while (std::sqrt(temp) - std::floor(std::sqrt(temp)) != 0) {
    temp++;
  }

  size_t num_levels_per_side = (size_t)std::sqrt(temp);

  // Takes the mouse coordinates and divides them by the size of a grid box
  // This converts them to numbers that can be used as indices.
  // Ex: an x-coordinate of 550 with a grid box size of 100 means the mouse is
  // in the 5th zero-indexed box.
  glm::vec2 mouse_pos = mouse_coords - top_left_corner_;
  float grid_box_size = side_length_ / num_levels_per_side;
  size_t col = (size_t)std::floor(mouse_pos.x / grid_box_size);
  size_t row = (size_t)std::floor(mouse_pos.y / grid_box_size);

  // Ex: The 2nd row, 2nd column of a 4x4 would be the 6th box or 5th when zero
  // indexed.
  size_t level_num = row * num_levels_per_side + col;
  current_level_ = levels_[level_num];
  ResetUserLayout();
  return level_num;
}

void Sketchpad::SetPipeNumber(const glm::vec2& mouse_coords) {
  glm::vec2 mouse_pos = mouse_coords - top_left_corner_;
  float grid_box_size = side_length_ / current_level_.GetDimension();
  size_t col = (size_t)std::floor(mouse_pos.x / grid_box_size);
  size_t row = (size_t)std::floor(mouse_pos.y / grid_box_size);

  if (pipe_number_ != user_layout_[row][col]) {
    num_moves_++;
  }

  pipe_number_ = user_layout_[row][col];
}

void Sketchpad::SetColor(const std::vector<float>& rgb) const {
  ci::gl::color(rgb[0], rgb[1], rgb[2]);
}

const Flow::Level& Sketchpad::GetLevel(size_t level_number) const {
  return levels_.at(level_number);
}

const std::vector<Flow::Level>& Sketchpad::GetAllLevels() const {
  return levels_;
}

void Sketchpad::ResetUserLayout() {
  user_layout_.clear();
  num_moves_ = 0;
  pipe_number_ = kBlankChar;
}

void Sketchpad::OnComplete(size_t level_number) {
  levels_[level_number].SetIsComplete();

  if (num_moves_ < levels_[level_number].GetBestScore() ||
      levels_[level_number].GetBestScore() == 0) {
    levels_[level_number].SetBestScore(num_moves_);
  }

  ci::gl::color(0, 0, 0);
  ci::gl::drawStringCentered(
      "Level Complete!",
      top_left_corner_ + glm::vec2(side_length_ / 2, side_length_ / 2),
      ci::Color("White"), ci::Font("Times New Roman", kCompleteText));
}

}  // namespace visualizer
}  // namespace Flow

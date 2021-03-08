#pragma once
#include <core/level.h>

#include <glm/vec2.hpp>
#include <vector>

namespace Flow {
namespace visualizer {

class Sketchpad {
 public:
  Sketchpad(const glm::vec2& top_left_corner, float side_length);

  /**
   * Draws a rectangle for the sketchpad.
   */
  void Draw() const;

  /**
   * Draws the current level through broken up methods. If the level is
   * completed, also updates best score and displayed a level complete message.
   * @param level_number The number of the desired level.
   */
  void DrawLevel(size_t level_number);

  /**
   * Draws a grid with a numbered box for each corresponding level. If a level
   * has been previously completed, a star is also drawn on top of the number.
   */
  void DrawLevelSelect() const;

  /**
   * Modifies the user layout based on the mouse movements. Every grid box the
   * mouse drags across will be modified to reflect what color it should be
   * drawn as by the DrawPath method.
   * @param mouse_coords The coordinates of the mouse at the moment it is being
   * dragged.
   */
  void HandleBrush(const glm::vec2& mouse_coords);

  /**
   * Modifies the pipe_number variable based on what circle the user clicked on.
   * Each circle corresponds to a number 1-9.
   * @param mouse_coords The coordinates of the mouse the moment the left mouse
   * button was clicked.
   */
  void SetPipeNumber(const glm::vec2& mouse_coords);

  /**
   * Modifies the current level variable based on what grid box is clicked in
   * the level select screen.
   * @param mouse_coords The coordinates of the mouse the moment the left mouse
   * button was clicked.
   * @return The number of the current level.
   */
  size_t SetCurrentLevel(const glm::vec2& mouse_coords);

  const Level& GetLevel(size_t level_number) const;
  const std::vector<Level>& GetAllLevels() const;

  /**
   * Resets number of moves to 0 and clears the user layout
   */
  void ResetUserLayout();

 private:
  glm::vec2 top_left_corner_;
  glm::vec2 bottom_left_corner_;
  float side_length_;

  // What the user sees on the app window and can draw on
  Layout user_layout_;
  std::vector<Level> levels_;
  Level current_level_;
  char pipe_number_;
  size_t num_moves_;

  const std::string kLevelFilePath =
      "c:/Users/Andori/Cinder/my-projects/final-project-andrewson3107/data/"
      "levels.txt";
  const char kBlankChar = '#';
  const std::string kCompleteChar = "☆";

  // Font size constants
  const float kCompleteText = 100;
  const float kStatText = 20;
  const float kLevelNumText = 80;
  const float kStarText = 110;

  // Spacing constants
  const float kStatMarginX = 40;
  const float kStatMarginY = 10;

  // 9 set of colors used for circles.
  const std::vector<std::vector<float>> kColors{
      {1, 0, 0}, {1, 1, 0},       {1, 0, 1},
      {0, 1, 0}, {0, 1, 1},       {1, 0.8f, 0.7f},
      {0, 0, 1}, {0.9f, 0.6f, 0}, {0.2f, 0.5f, 0.5f},
  };

  /**
   * Reads in levels from a specific file. Creates level objects and stores them
   * in levels_
   * @param file_path The path of the file to read levels from.
   */
  void ReadLevels(const std::string& file_path);

  /**
   * Helper method that draws the grid lines within the sketchpad rectangle
   * based on the dimension of the level. A 5x5 level will require 3 grid lines
   * horizontally and vertically.
   */
  void DrawGrid() const;

  /**
   * Draws the colored circled depending on where they are in the level in the
   * appropriate grid box on the app window.
   */
  void DrawCircles() const;

  /**
   * Draws the paths between two boxes on the grid. Paths are created when the
   * user drags the mouse from one square to another. The path consists of a
   * circle and rectangles connecting to the adjacent grid box.
   */
  void DrawPaths() const;

  /**
   * Draws the user stats below the grid. Stats include number of moves used,
   * best score achieved, and % of level completed.
   */
  void DrawStats() const;

  /**
   * Helper method to modify the color variable depending on what circle is
   * being drawn.
   * @param rgb A vector of 3 float values coresponding to rgb values.
   */
  void SetColor(const std::vector<float>& rgb) const;

  /**
   * Helper method called when a level is completed. Displays a message and
   * updates the levels is_complete boolean and best score.
   * @param level_number The current level number.
   */
  void OnComplete(size_t level_number);
};
}  // namespace visualizer
}  // namespace Flow
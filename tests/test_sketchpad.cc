#include <visualizer/sketchpad.h>

#include <catch2/catch.hpp>
#include <iostream>

using Flow::visualizer::Sketchpad;

TEST_CASE("Loads levels properly") {
  Sketchpad sketchpad(glm::vec2(200, 200), 200);
  Flow::Layout initial{{'1', '#', '#', '#', '1'},
                       {'2', '#', '#', '#', '2'},
                       {'3', '#', '#', '#', '3'},
                       {'4', '#', '#', '#', '4'},
                       {'5', '#', '#', '#', '5'}};
  Flow::Layout solution{{'1', '1', '1', '1', '1'},
                       {'2', '2', '2', '2', '2'},
                       {'3', '3', '3', '3', '3'},
                       {'4', '4', '4', '4', '4'},
                       {'5', '5', '5', '5', '5'}};
  REQUIRE(sketchpad.GetLevel(0).GetInitialState() == initial);
  REQUIRE(sketchpad.GetLevel(0).GetSolution() == solution);
}

TEST_CASE("Levels of different size") {
  Sketchpad sketchpad(glm::vec2(200, 200), 200);
  Flow::Layout initial{{'1', '#', '#', '#', '#', '#', '#', '#', '1'},
                       {'2', '#', '#', '#', '#', '#', '#', '#', '2'},
                       {'3', '#', '#', '#', '#', '#', '#', '#', '3'},
                       {'4', '#', '#', '#', '#', '#', '#', '#', '4'},
                       {'5', '#', '#', '#', '#', '#', '#', '#', '5'},
                       {'6', '#', '#', '#', '#', '#', '#', '#', '6'},
                       {'7', '#', '#', '#', '#', '#', '#', '#', '7'},
                       {'8', '#', '#', '#', '#', '#', '#', '#', '8'},
                       {'9', '#', '#', '#', '#', '#', '#', '#', '9'}};
  Flow::Layout solution{{'1', '1', '1', '1', '1', '1', '1', '1', '1'},
                        {'2', '2', '2', '2', '2', '2', '2', '2', '2'},
                        {'3', '3', '3', '3', '3', '3', '3', '3', '3'},
                        {'4', '4', '4', '4', '4', '4', '4', '4', '4'},
                        {'5', '5', '5', '5', '5', '5', '5', '5', '5'},
                        {'6', '6', '6', '6', '6', '6', '6', '6', '6'},
                        {'7', '7', '7', '7', '7', '7', '7', '7', '7'},
                        {'8', '8', '8', '8', '8', '8', '8', '8', '8'},
                        {'9', '9', '9', '9', '9', '9', '9', '9', '9'}};
  REQUIRE(sketchpad.GetLevel(15).GetInitialState() == initial);
  REQUIRE(sketchpad.GetLevel(15).GetSolution() == solution);
}

TEST_CASE("Setting Level") {
  Sketchpad sketchpad(glm::vec2(0, 0), 400);

  for (size_t row = 0; row < 4; row++) {
    for (size_t col = 0; col < 4; col++) {
      glm::vec2 mouse_coords = glm::vec2(100*col + 1,100*row + 1);
      REQUIRE(sketchpad.SetCurrentLevel(mouse_coords) == row*4 + col);
    }
  }
}
#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "sketchpad.h"

namespace Flow {

namespace visualizer {

const std::string kFont = "Times New Roman";
enum Screen { kMainMenu, kLevelSelect, kLevel, kDeveloper };

class FlowApp : public ci::app::App {
 public:
  FlowApp();
  /**
   * Sets the background color then calls the appropriate draw method
   * based on the current screen enum
   */
  void draw() override;

  /**
   * Draws a play, developer, and exit button.
   */
  void DrawMainMenu() const;

  /**
   * Draws the current level on the sketchpad. Also titles the level with it's
   * number and size.
   */
  void DrawLevel();

  /**
   * Draws a numbered grid corresponding to each level. Each grid block is
   * clickable and will take you to the respective level.
   */
  void DrawLevelSelect() const;

  /**
   * Draws a page dedicated to AndrewSon3107.
   */
  void DrawDeveloperPage() const;

  /**
   * Helper method that draws a back button in the top left corner. The back
   * button will take you to the main menu screen.
   */
  void DrawBackButton() const;

  /**
   * Helper method for testing the functionality of each level. When either the
   * left or right arrow key is pressed, the current level will change
   * respectively. The delete key will delete anything drawn by the user.
   * @param event The event corresponding to the key pressed.
   */
  void keyDown(ci::app::KeyEvent event) override;

  /**
   * Provides functionality to each button depending on mouse
   * location. If mouse location falls within a specific button, the proper
   * method is executed.
   * When the current screen is the level, mouse down tracks the color of the
   * circle clicked.
   * @param event The event containing mouse coordinates on click
   */
  void mouseDown(ci::app::MouseEvent event) override;

  /**
   * Tracks mouse movement and stores which part of the grid the mouse travels
   * across. Stores them later to be drawn as paths.
   * @param event The event containing mouse coordinates on click
   */
  void mouseDrag(ci::app::MouseEvent event) override;

  const float kMargin = 100;
  const float kWindowWidth = 800;
  const float kWindowLength = 800;  // Length is x width is y
  const float kSketchpadSize = 600;

 private:
  Sketchpad sketchpad_;
  size_t current_level_num_;
  Screen current_screen_;

  // Font constants
  const float kHeader1 = 120;
  const float kHeader2 = 70;
  const float kRegularTextSize = 75;
  const float kSmallTextSize = 30;
  const float kSymbolTextSize = 120;

  // Button constants
  const float kRegularButtonWidth = 80;
  const float kRegularButtonLength = 250;
  const float kXAlignment = kWindowLength / 2 - kRegularButtonLength / 2;

  const glm::vec2 kButtonSize =
      glm::vec2(kRegularButtonLength, kRegularButtonWidth);
  const glm::vec2 kBackButtonSize = glm::vec2(70, 70);

  // Button coordinates (with respect to top-left corner)
  const glm::vec2 kPlayButton = glm::vec2(kXAlignment, 200);
  const glm::vec2 kOptionsButton = glm::vec2(kXAlignment, 400);
  const glm::vec2 kExitButton = glm::vec2(kXAlignment, 600);
  const glm::vec2 kBackButton = glm::vec2(10, 10);

  // Text coordinates
  const glm::vec2 kTitleTextPos = glm::vec2(kWindowLength / 2, 50);
  const glm::vec2 kBackTextPos = glm::vec2(45, 7);
};

}  // namespace visualizer
}  // namespace Flow
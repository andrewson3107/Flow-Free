#include <visualizer/flow_app.h>

namespace Flow {

namespace visualizer {

FlowApp::FlowApp()
    : sketchpad_(Sketchpad(glm::vec2(kMargin, kMargin), kSketchpadSize)),
      current_screen_(kMainMenu) {  // Change this for initial screen
  ci::app::setWindowSize((int)kWindowLength, (int)kWindowWidth);
}

void FlowApp::draw() {
  ci::Color8u background_color(189, 202, 252);
  ci::gl::clear(background_color);

  switch (current_screen_) {
    case kMainMenu:
      DrawMainMenu();
      break;
    case kLevelSelect:
      DrawLevelSelect();
      break;
    case kLevel:
      DrawLevel();
      break;
    case kDeveloper:
      DrawDeveloperPage();
      break;
  }
}

void FlowApp::DrawMainMenu() const {
  ci::gl::drawStringCentered("Flow Free", kTitleTextPos, ci::Color("Black"),
                             ci::Font(kFont, kHeader1));

  ci::Color8u button_color(62, 64, 71);
  ci::gl::color(button_color);

  ci::gl::drawSolidRect(ci::Rectf(kPlayButton, kPlayButton + kButtonSize));

  ci::gl::drawSolidRect(
      ci::Rectf(kOptionsButton, kOptionsButton + kButtonSize));

  ci::gl::drawSolidRect(ci::Rectf(kExitButton, kExitButton + kButtonSize));

  ci::gl::drawStringCentered(
      "Play", glm::vec2(kWindowLength / 2, kPlayButton.y), ci::Color("Black"),
      ci::Font(kFont, kRegularTextSize));

  ci::gl::drawStringCentered(
      "Dev Page", glm::vec2(kWindowLength / 2, kOptionsButton.y),
      ci::Color("Black"), ci::Font(kFont, kRegularTextSize));

  ci::gl::drawStringCentered(
      "Exit", glm::vec2(kWindowLength / 2, kExitButton.y), ci::Color("Black"),
      ci::Font(kFont, kRegularTextSize));
}

void FlowApp::DrawLevel() {
  DrawBackButton();

  sketchpad_.Draw();
  sketchpad_.DrawLevel(current_level_num_);

  std::string level_header =
      "Level: " + std::to_string(current_level_num_ + 1) + "    " +
      std::to_string(sketchpad_.GetLevel(current_level_num_).GetDimension()) +
      "x" +
      std::to_string(sketchpad_.GetLevel(current_level_num_).GetDimension());

  ci::gl::drawStringCentered(level_header, kTitleTextPos, ci::Color("Black"),
                             ci::Font(kFont, kHeader2));

}

void FlowApp::DrawLevelSelect() const {
  DrawBackButton();

  ci::gl::drawStringCentered("Level Select", kTitleTextPos, ci::Color("Black"),
                             ci::Font(kFont, kHeader2));
  sketchpad_.Draw();
  sketchpad_.DrawLevelSelect();
}

void FlowApp::DrawDeveloperPage() const {
  DrawBackButton();

  ci::gl::drawStringCentered("Created by Andrew Son for CS 126. Thank you for playing.", kTitleTextPos, ci::Color("Black"),
                             ci::Font(kFont, kSmallTextSize));
}

void FlowApp::DrawBackButton() const {
  ci::Color8u button_color(0, 0, 0);
  ci::gl::color(button_color);
  ci::gl::drawSolidRect(ci::Rectf(kBackButton, kBackButton + kBackButtonSize));
  ci::gl::drawStringCentered("⇦", kBackTextPos, ci::Color("White"),
                             ci::Font(kFont, kSymbolTextSize));
}

void FlowApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_RIGHT:
      if (current_level_num_ + 1 == sketchpad_.GetAllLevels().size()) {
        break;
      }
      current_level_num_++;
      sketchpad_.ResetUserLayout();
      break;

    case ci::app::KeyEvent::KEY_LEFT:
      if (current_level_num_ == 0) {
        break;
      }
      current_level_num_--;
      sketchpad_.ResetUserLayout();
      break;

    case ci::app::KeyEvent::KEY_DELETE:
      sketchpad_.ResetUserLayout();
  }
}

void FlowApp::mouseDown(ci::app::MouseEvent event) {
  glm::vec2 coordinates = event.getPos();

  switch (current_screen_) {
    case kMainMenu:
      if (coordinates.x > kPlayButton.x &&
          coordinates.x < (kPlayButton + kButtonSize).x &&
          coordinates.y > kPlayButton.y &&
          coordinates.y < (kPlayButton + kButtonSize).y) {
        current_screen_ = kLevelSelect;
      }

      if (coordinates.x > kOptionsButton.x &&
          coordinates.x < (kOptionsButton + kButtonSize).x &&
          coordinates.y > kOptionsButton.y &&
          coordinates.y < (kOptionsButton + kButtonSize).y) {
        current_screen_ = kDeveloper;
      }

      if (coordinates.x > kExitButton.x &&
          coordinates.x < (kExitButton + kButtonSize).x &&
          coordinates.y > kExitButton.y &&
          coordinates.y < (kExitButton + kButtonSize).y) {
        quit();
      }
      break;

    case kLevelSelect:
      if (coordinates.x > kMargin && coordinates.x < kWindowLength - kMargin &&
          coordinates.y > kMargin && coordinates.y < kWindowWidth - kMargin) {
        current_level_num_ = sketchpad_.SetCurrentLevel(coordinates);
        current_screen_ = kLevel;
      }

      if (coordinates.x > kBackButton.x &&
          coordinates.x < (kBackButton + kBackButtonSize).x &&
          coordinates.y > kBackButton.y &&
          coordinates.y < (kBackButton + kButtonSize).y) {
        current_screen_ = kMainMenu;
      }
      break;

    case kLevel:
      if (coordinates.x > kMargin && coordinates.x < kWindowLength - kMargin &&
          coordinates.y > kMargin && coordinates.y < kWindowWidth - kMargin) {
        sketchpad_.SetPipeNumber(coordinates);
      }

      if (coordinates.x > kBackButton.x &&
          coordinates.x < (kBackButton + kBackButtonSize).x &&
          coordinates.y > kBackButton.y &&
          coordinates.y < (kBackButton + kButtonSize).y) {
        current_screen_ = kLevelSelect;
      }
      break;

    case kDeveloper:
      if (coordinates.x > kBackButton.x &&
          coordinates.x < (kBackButton + kBackButtonSize).x &&
          coordinates.y > kBackButton.y &&
          coordinates.y < (kBackButton + kButtonSize).y) {
        current_screen_ = kMainMenu;
      }
      break;
  }
}

void FlowApp::mouseDrag(ci::app::MouseEvent event) {
  glm::vec2 coordinates = event.getPos();

  switch (current_screen_) {
    case kLevel:
      if (coordinates.x > kMargin && coordinates.x < kWindowLength - kMargin &&
          coordinates.y > kMargin && coordinates.y < kWindowWidth - kMargin) {
        sketchpad_.HandleBrush(coordinates);
      }
      break;
  }
}

}  // namespace visualizer
}  // namespace Flow
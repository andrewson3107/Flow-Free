#include <visualizer/flow_app.h>

using Flow::visualizer::FlowApp;

void prepareSettings(FlowApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(FlowApp, ci::app::RendererGl, prepareSettings);

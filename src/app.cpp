#include "app.hpp"
#include "renderer.hpp"
#include "window.hpp"

namespace reach {

App::App() : window(std::make_unique<Window>()), renderer(std::make_unique<Renderer>()) {}

void App::run() {
    while (!window->should_close()) {
        window->frame();
    }
}

} // namespace reach

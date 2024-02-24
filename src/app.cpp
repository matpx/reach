#include "app.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include "world.hpp"

namespace reach {

App::App()
    : window(std::make_unique<Window>()), renderer(std::make_unique<Renderer>()), world(std::make_unique<World>()) {}

void App::run() {
    while (!window->should_close()) {
        window->frame();
    }
}

} // namespace reach

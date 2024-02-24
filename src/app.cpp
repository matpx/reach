#include "app.hpp"
#include <memory>

namespace reach {

App::App() : window(std::make_unique<Window>()) {}

void App::run() {
    while (!window->should_close()) {
        window->frame();
    }
}

} // namespace reach

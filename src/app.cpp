#include "app.hpp"
#include "manager/material_manager.hpp"
#include "manager/renderer_manager.hpp"
#include "manager/window_manager.hpp"
#include "world.hpp"

namespace reach {

App::App()
    : window(std::make_unique<WindowManager>()), renderer(std::make_unique<RendererManager>()),
      material(std::make_unique<MaterialManager>()), world(std::make_unique<World>()) {}

void App::run() {
    while (!window->should_close()) {
        world->update_transforms();

        window->finish_frame();
    }
}

} // namespace reach

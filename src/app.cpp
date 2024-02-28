#include "app.hpp"
#include "manager/device_manager.hpp"
#include "manager/material_manager.hpp"
#include "manager/window_manager.hpp"
#include "systems/render_system.hpp"
#include "systems/transform_system.hpp"
#include "world.hpp"

namespace reach {

App::App()
    : window(std::make_unique<WindowManager>()), renderer(std::make_unique<DeviceManager>()),
      material(std::make_unique<MaterialManager>()), world(std::make_unique<World>()) {}

void App::run() {
    while (!window->should_close()) {
        transform_system::update(*world);
        render_system::update();

        window->finish_frame();
    }
}

} // namespace reach

#include <app.hpp>
#include <manager/device_manager.hpp>
#include <manager/material_manager.hpp>
#include <manager/model_manager.hpp>
#include <manager/window_manager.hpp>
#include <systems/player_system.hpp>
#include <systems/render_system.hpp>
#include <systems/transform_system.hpp>
#include <utils/log.hpp>
#include <world.hpp>

namespace reach {

App::App()
    : window_manager(std::make_unique<WindowManager>(glm::ivec2{1200, 800})), device_manager(std::make_unique<DeviceManager>()),
      material_manager(std::make_unique<MaterialManager>()), model_manager(std::make_unique<ModelManager>()),
      world(std::make_unique<World>()) {}

void App::run() {
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#endif

    LOG_DEBUG("debug mode!");

    while (!window_manager->should_close()) {
        transform_system::update();
        player_system::update();
        render_system::update();

        window_manager->finish_frame();
    }
}

} // namespace reach

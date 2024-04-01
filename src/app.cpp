#include <app.hpp>
#include <manager/device_manager.hpp>
#include <manager/input_manager.hpp>
#include <manager/material_manager.hpp>
#include <manager/model_manager.hpp>
#include <manager/ui_manager.hpp>
#include <manager/window_manager.hpp>
#include <systems/player_system.hpp>
#include <systems/render_system.hpp>
#include <systems/transform_system.hpp>
#include <utils/log.hpp>
#include <world.hpp>

namespace reach {

App::App()
    : input_manager(std::make_unique<InputManager>()), window_manager(std::make_unique<WindowManager>(glm::ivec2{1600, 1000})),
      device_manager(std::make_unique<DeviceManager>()), material_manager(std::make_unique<MaterialManager>()),
      model_manager(std::make_unique<ModelManager>()), ui_manager(std::make_unique<UiManager>()), world(std::make_unique<World>()) {}

void App::run() {
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#endif

    LOG_DEBUG("debug mode!");

    while (!window_manager->should_close()) {
        window_manager->poll();
        
        transform_system::update();
        player_system::update();

        render_system::post_update();

        window_manager->swap();
    }
}

} // namespace reach

#include <app.hpp>
#include <chrono>
#include <manager/device_manager.hpp>
#include <manager/input_manager.hpp>
#include <manager/material_manager.hpp>
#include <manager/model_manager.hpp>
#include <manager/ui_manager.hpp>
#include <manager/window_manager.hpp>
#include <systems/player_system.hpp>
#include <systems/render_system.hpp>
#include <systems/terrain_system.hpp>
#include <systems/transform_system.hpp>
#include <utils/log.hpp>
#include <world.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

namespace reach {

std::function<void()> main_loop;
void wasm_main_loop() { main_loop(); }

App::App()
    : input_manager(std::make_unique<InputManager>()), window_manager(std::make_unique<WindowManager>(glm::ivec2{1600, 1000})),
      device_manager(std::make_unique<DeviceManager>()), material_manager(std::make_unique<MaterialManager>()),
      model_manager(std::make_unique<ModelManager>()), ui_manager(std::make_unique<UiManager>()), world(std::make_unique<World>()) {}

void App::run() {
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#endif

    LOG_DEBUG("debug mode!");

    auto last_time = std::chrono::high_resolution_clock::now();

    main_loop = [&] {
        window_manager->poll();

        const auto current_time = std::chrono::high_resolution_clock::now();
        const float delta_time = std::chrono::duration<float, std::milli>(current_time - last_time).count() / 1000.0f;
        last_time = current_time;

        transform_system::update();
        terrain_system::update();
        player_system::update(delta_time);

        render_system::post_update();

        window_manager->swap();
    };

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(wasm_main_loop, 0, true);
#else
    while (!window_manager->should_close()) {
        main_loop();
    }
#endif
}

} // namespace reach

#include <app.hpp>
#include <manager/device_manager.hpp>
#include <manager/input_manager.hpp>
#include <manager/material_manager.hpp>
#include <manager/model_manager.hpp>
#include <manager/physics_manager.hpp>
#include <manager/ui_manager.hpp>
#include <manager/window_manager.hpp>
#include <utils/log.hpp>
#include <world.hpp>

using namespace reach;

int main() {
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#endif

    LOG_DEBUG("debug mode!");

    App *app = new App;

    app->run();

    delete app;
}

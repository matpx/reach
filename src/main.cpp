#include <app.hpp>
#include <manager/device_manager.hpp>
#include <manager/material_manager.hpp>
#include <manager/window_manager.hpp>
#include <world.hpp>

using namespace reach;

int main() {
    App *app = new App;

    app->run();

    delete app;
}

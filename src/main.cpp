#include "app.hpp"
#include "renderer.hpp"
#include "window.hpp"

using namespace reach;

int main() {
    App *app = new App;

    app->run();

    delete app;
}

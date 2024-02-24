#include "app.hpp"
#include "glm/ext/matrix_float2x2.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include "world.hpp"


using namespace reach;

int main() {
    App *app = new App;

    app->run();

    delete app;
}

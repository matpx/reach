#include "app.hpp"

using namespace reach;

int main() {
    App *app = new App;

    app->run();

    delete app;
}

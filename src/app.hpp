#pragma once

#include "nocopy.hpp"
#include <memory>

namespace reach {

class Window;
class Renderer;
class World;

class App : NoCopy {
    private:
        std::unique_ptr<Window> window;
        std::unique_ptr<Renderer> renderer;
        std::unique_ptr<World> world;

    public:
        App();
        void run();
};

} // namespace reach

#pragma once

#include "nocopy.hpp"
#include <memory>

namespace reach {

class Window;
class Renderer;

class App : NoCopy {
    private:
        std::unique_ptr<Window> window;
        std::unique_ptr<Renderer> renderer;

    public:
        App();
        void run();
};

} // namespace reach

#pragma once

#include "nocopy.hpp"
#include "window.hpp"
#include <memory>

namespace reach {

class App : NoCopy {
    private:
        std::unique_ptr<Window> window;

    public:
        App();
        void run();
};

} // namespace reach

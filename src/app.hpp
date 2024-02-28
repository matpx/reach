#pragma once

#include "nocopy.hpp"
#include <memory>

namespace reach {

class WindowManager;
class DeviceManager;
class MaterialManager;
class World;

class App : NoCopy {
    private:
        std::unique_ptr<WindowManager> window;
        std::unique_ptr<DeviceManager> renderer;
        std::unique_ptr<MaterialManager> material;
        std::unique_ptr<World> world;

    public:
        App();
        void run();
};

} // namespace reach

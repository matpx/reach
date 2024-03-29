#pragma once

#include <memory>
#include <utils/nocopy.hpp>

namespace reach {

class WindowManager;
class DeviceManager;
class MaterialManager;
class World;

class App : NoCopy {
    private:
        std::unique_ptr<WindowManager> window_manager;
        std::unique_ptr<DeviceManager> device_manager;
        std::unique_ptr<MaterialManager> material_manager;
        std::unique_ptr<World> world;

    public:
        App();
        void run();
};

} // namespace reach

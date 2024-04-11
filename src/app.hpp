#pragma once

#include <memory>
#include <utils/nocopy.hpp>

namespace reach {

class InputManager;
class WindowManager;
class PhysicsManager;
class DeviceManager;
class MaterialManager;
class ModelManager;
class UiManager;
class World;

class App : NoCopy {
    private:
        std::unique_ptr<InputManager> input_manager;
        std::unique_ptr<WindowManager> window_manager;
        std::unique_ptr<PhysicsManager> physics_manager;
        std::unique_ptr<DeviceManager> device_manager;
        std::unique_ptr<MaterialManager> material_manager;
        std::unique_ptr<ModelManager> model_manager;
        std::unique_ptr<UiManager> ui_manager;
        std::unique_ptr<World> world;

    public:
        App();
        void run();
};

} // namespace reach

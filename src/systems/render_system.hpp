#pragma once

namespace reach {

class DeviceManager;
class MaterialManager;

} // namespace reach

namespace reach::render_system {

void update(DeviceManager &device_manager, MaterialManager &material_manager);

}

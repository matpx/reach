#pragma once

#include <manager/manager.hpp>

namespace reach {

struct MeshComponent;

class DeviceManager final : public Manager {
    public:
        DeviceManager();
        ~DeviceManager();

        void update_mesh(MeshComponent &mesh_component);
        void unload_mesh(MeshComponent &mesh_component);
};

} // namespace reach
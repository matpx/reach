#pragma once

#include <manager/manager.hpp>

namespace reach {

struct MeshComponent;
struct MaterialComponent;

class DeviceManager final : public Manager {
    public:
        DeviceManager();
        ~DeviceManager();

        void update_mesh(MeshComponent &mesh_component);
        void unload_mesh(MeshComponent &mesh_component);

        void begin_frame();
        void draw_mesh(const MaterialComponent &material, const MeshComponent &mesh_component);
        void finish_frame();
};

} // namespace reach
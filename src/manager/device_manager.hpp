#pragma once

#include <manager/manager.hpp>

namespace reach {

struct MeshComponent;
struct MaterialComponent;
struct TransformComponent;

class DeviceManager final : public Manager {
    private:
        bool pass_is_active = false;

    public:
        [[nodiscard]] static DeviceManager &get();

        DeviceManager();
        ~DeviceManager();

        void update_mesh(MeshComponent &mesh_component);
        void unload_mesh(MeshComponent &mesh_component);

        void begin_main_pass();
        void draw_mesh(const TransformComponent &transform_component, const MaterialComponent &material_component,
                       const MeshComponent &mesh_component);
        void finish_main_pass();
};

} // namespace reach
#pragma once

#include <manager/manager.hpp>
#include <mat4x4.hpp>

namespace reach {

struct MeshData;
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

        void upload_meshdata(MeshData &mesh_data);
        void unload_meshdata(MeshData &mesh_data);

        void begin_main_pass();
        void draw_mesh(const glm::mat4 &model_view_projection, const MaterialComponent &material_component,
                       const MeshComponent &mesh_component);
        void finish_main_pass();
};

} // namespace reach
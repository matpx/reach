#include <components/mesh_component.hpp>
#include <manager/device_manager.hpp>
#include <manager/material_manager.hpp>
#include <systems/render_system.hpp>

namespace reach::render_system {

void update(DeviceManager &device_manager, MaterialManager &material_manager) {
    static bool first_run = true;

    static MeshComponent triangle_mesh = {
        ._debug_name = "triangle mesh",
    };

    if (first_run) {
        first_run = false;

        triangle_mesh.mesh_data = std::make_shared<MeshData>(MeshData{
            .vertex_data = {{{0, 0, 0}}, {{1, 0, 0}}, {{1, 1, 0}}},
            .index_data = {2, 1, 0},
        });

        device_manager.update_mesh(triangle_mesh);
    }

    device_manager.begin_frame();
    device_manager.draw_mesh(material_manager.unlit_material, triangle_mesh);
    device_manager.finish_frame();
}

} // namespace reach::render_system

#include <components/mesh_component.hpp>
#include <manager/device_manager.hpp>
#include <manager/material_manager.hpp>
#include <systems/render_system.hpp>

namespace reach::render_system {

void update() {
    static bool first_run = true;

    static MeshComponent triangle_mesh = {
        .debug_name = "triangle mesh",
    };

    if (first_run) {
        first_run = false;

        triangle_mesh.mesh_data = std::make_shared<MeshData>(MeshData{
            .vertex_data = {{{0, 0, 0}}, {{1, 0, 0}}, {{1, 1, 0}}},
            .index_data = {2, 1, 0},
        });

        DeviceManager::get().update_mesh(triangle_mesh);
    }

    DeviceManager::get().begin_frame();
    DeviceManager::get().draw_mesh(MaterialManager::get().unlit_material, triangle_mesh);
    DeviceManager::get().finish_frame();
}

} // namespace reach::render_system

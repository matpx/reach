#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <manager/device_manager.hpp>
#include <manager/material_manager.hpp>
#include <systems/render_system.hpp>
#include <world.hpp>

namespace reach::render_system {

void update() {
    static bool first_run = true;

    if (first_run) {
        first_run = false;

        MeshComponent triangle_mesh = {
            .debug_name = "triangle mesh",
        };

        triangle_mesh.mesh_data = std::make_shared<MeshData>(MeshData{
            .vertex_data = {{{0, 0, 0}}, {{1, 0, 0}}, {{1, 1, 0}}},
            .index_data = {2, 1, 0},
        });

        DeviceManager::get().update_mesh(triangle_mesh);

        const auto triangle_entity = World::current().create();
        World::current().emplace<TransformComponent>(triangle_entity, TransformComponent{});
        World::current().emplace<MaterialComponent>(triangle_entity, MaterialManager::get().unlit_material);
        World::current().emplace<MeshComponent>(triangle_entity, triangle_mesh);
    }

    DeviceManager::get().begin_frame();

    for (auto [entity, transform, material, mesh] :
         World::current().view<TransformComponent, MaterialComponent, MeshComponent>().each()) {
        DeviceManager::get().draw_mesh(transform, material, mesh);
    }

    DeviceManager::get().finish_frame();
}

} // namespace reach::render_system

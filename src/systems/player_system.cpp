#include <components/camera_component.hpp>
#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <manager/device_manager.hpp>
#include <manager/material_manager.hpp>
#include <systems/player_system.hpp>
#include <world.hpp>

namespace reach::player_system {

void update() {
    auto &world = World::current();

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

        const auto triangle_entity = world.create();
        world.emplace<TransformComponent>(triangle_entity, TransformComponent{});
        world.emplace<MaterialComponent>(triangle_entity, MaterialManager::get().unlit_material);
        if (DeviceManager::get().update_mesh(triangle_mesh)) {
            world.emplace<MeshComponent>(triangle_entity, triangle_mesh);
        }

        world.current_camera = world.create();
        world.emplace<TransformComponent>(world.current_camera, TransformComponent{.translation = {0.0f, 0.0f, 5.0f}});
        world.emplace<CameraComponent>(world.current_camera, CameraComponent::make(1.4f, 1.0f, 0.1f, 100.0f));
    }
}

} // namespace reach::player_system

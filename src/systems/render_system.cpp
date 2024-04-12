#include <components/camera_component.hpp>
#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <manager/device_manager.hpp>
#include <manager/material_manager.hpp>
#include <manager/ui_manager.hpp>
#include <systems/render_system.hpp>
#include <world.hpp>

namespace reach::render_system {

void post_update() {
    auto &world = World::current();

    DeviceManager::get().begin_main_pass();

    glm::mat4 view_projection = glm::identity<glm::mat4>();

    if (world.current_camera != entt::null) {
        view_projection = world.get<CameraComponent>(world.current_camera).perspective_proj *
                          glm::inverse(world.get<TransformComponent>(world.current_camera).world);
    }

    for (auto [entity, transform, material, mesh] : world.view<TransformComponent, MaterialComponent, MeshComponent>().each()) {
        const glm::mat4 model_view_projection = view_projection * transform.world;

        if (!mesh.visible) {
            continue;
        }

        // if (material.pipeline.id == SG_INVALID_ID) {
        //     material = MaterialManager::get().unlit_material;
        // }

        if (!mesh.mesh_data->is_uploaded()) {
            DeviceManager::get().upload_meshdata(*mesh.mesh_data);
        }

        DeviceManager::get().draw_mesh(model_view_projection, material, mesh);
    }

    UiManager::get().submit_to_device();

    DeviceManager::get().finish_main_pass();
}

} // namespace reach::render_system

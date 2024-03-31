#include <components/camera_component.hpp>
#include <components/transform_component.hpp>
#include <manager/model_manager.hpp>
#include <manager/ui_manager.hpp>
#include <manager/window_manager.hpp>
#include <systems/player_system.hpp>
#include <utils/log.hpp>
#include <world.hpp>

namespace reach::player_system {

void update() {
    auto &world = World::current();

    static bool first_run = true;
    static entt::entity player = entt::null;

    if (first_run) {
        first_run = false;

        ModelManager::get()
            .load_gltf("assets/glb/challenger.glb")
            .and_then([](std::shared_ptr<Prefab> prefab) -> tl::expected<std::shared_ptr<Prefab>, std::string> {
                player = ModelManager::get().instantiate(World::current(), prefab);

                return prefab;
            })
            .or_else([](std::string error) { LOG_ERROR("Failed to load glb: {}", error); });

        world.current_camera = world.create();
        world.emplace<TransformComponent>(world.current_camera, TransformComponent{});

        world.emplace<CameraComponent>(world.current_camera,
                                       CameraComponent::make(1.4f, WindowManager::get().get_framebuffer_width_height(), 0.1f, 100.0f));
    }

    auto &camera_transform = world.get<TransformComponent>(World::current().current_camera);

    static float c = 0;
    c += 0.01f;

    camera_transform.translation = glm::vec3{sin(c) * 10, 1, cos(c) * 10};
    camera_transform.rotation = glm::quatLookAt(glm::normalize(-camera_transform.translation), glm::vec3{0, 1, 0});

    World::current().get<TransformComponent>(player).translation.y = sin(c) * 2;

    UiManager::get().draw_rect(glm::vec2{0, 0}, glm::vec2{200, 200});
    UiManager::get().draw_circle(glm::vec2{500, 500}, 100);
}

} // namespace reach::player_system

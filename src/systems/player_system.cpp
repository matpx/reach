#include <components/camera_component.hpp>
#include <components/transform_component.hpp>
#include <manager/input_manager.hpp>
#include <manager/model_manager.hpp>
#include <manager/ui_manager.hpp>
#include <manager/window_manager.hpp>
#include <systems/player_system.hpp>
#include <utils/log.hpp>
#include <world.hpp>

namespace reach::player_system {

void update(const float delta_time) {
    auto &world = World::current();

    const static entt::entity player = World::current().create();

    static bool first_run = true;

    if (first_run) {
        first_run = false;

        ModelManager::get()
            .load_gltf("assets/glb/challenger.glb")
            .and_then([](std::shared_ptr<Prefab> prefab) -> tl::expected<std::shared_ptr<Prefab>, std::string> {
                ModelManager::get().instantiate(World::current(), player, *prefab);

                return prefab;
            })
            .or_else([](std::string error) { LOG_ERROR("Failed to load glb: {}", error); });

        world.current_camera = world.create();
        world.emplace<TransformComponent>(world.current_camera, TransformComponent{.translation = {0, 0, 10}});
        world.emplace<CameraComponent>(world.current_camera,
                                       CameraComponent::make(1.4f, WindowManager::get().get_framebuffer_width_height(), 0.1f, 100.0f));
    }

    {
        auto &camera_transform = world.get<TransformComponent>(World::current().current_camera);

        const auto &input = InputManager::get();

        // rotation

        static glm::vec2 camera_rotation = {};

        camera_rotation += input.get_mouse_position_delta() * 0.003f;
        camera_rotation.y = glm::clamp(camera_rotation.y, -glm::pi<float>() / 2, glm::pi<float>() / 2);

        camera_transform.rotation =
            glm::angleAxis(camera_rotation.x, glm::vec3{0, -1, 0}) * glm::angleAxis(camera_rotation.y, glm::vec3{-1, 0, 0});

        // translation

        const float speed = 10.0f * delta_time;
        glm::vec3 velocity = {};

        if (input.is_action_pressed(InputAction::UP)) {
            velocity.z -= speed;
        } else if (input.is_action_pressed(InputAction::DOWN)) {
            velocity.z += speed;
        }

        if (input.is_action_pressed(InputAction::LEFT)) {
            velocity.x -= speed;
        } else if (input.is_action_pressed(InputAction::RIGHT)) {
            velocity.x += speed;
        }

        camera_transform.translation += camera_transform.rotation * velocity;
    }
}

} // namespace reach::player_system

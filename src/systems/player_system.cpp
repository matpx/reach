#include <components/camera_component.hpp>
#include <components/transform_component.hpp>
#include <manager/model_manager.hpp>
#include <manager/window_manager.hpp>
#include <systems/player_system.hpp>
#include <world.hpp>


namespace reach::player_system {

void update() {
    auto &world = World::current();

    static bool first_run = true;

    if (first_run) {
        first_run = false;

        ModelManager::get()
            .load_gltf("assets/glb/test.glb")
            .and_then([](std::shared_ptr<Prefab> prefab) -> tl::expected<std::shared_ptr<Prefab>, std::string> {
                ModelManager::get().instantiate(World::current(), prefab);

                return prefab;
            });

        world.current_camera = world.create();
        world.emplace<TransformComponent>(world.current_camera, TransformComponent{.translation = {0.0f, 0.0f, 10.0f}});
        world.emplace<CameraComponent>(world.current_camera,
                                       CameraComponent::make(1.4f, WindowManager::get().get_framebuffer_aspect(), 0.1f, 100.0f));
    }
}

} // namespace reach::player_system

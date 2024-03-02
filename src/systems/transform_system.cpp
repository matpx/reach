#include <components/transform_component.hpp>
#include <systems/render_system.hpp>
#include <world.hpp>

namespace reach::transform_system {

void update(World &world) {
    for (auto [entity, transform] : world.view<TransformComponent>().each()) {
        transform.model =
            glm::translate(glm::identity<glm::mat4>(), transform.translation) * glm::toMat4(transform.rotation);
    }
}

} // namespace reach::transform_system

#include <components/transform_component.hpp>
#include <world.hpp>

namespace reach::transform_system {

void update() {
    for (auto [entity, transform] : World::current().view<TransformComponent>().each()) {
        transform.model =
            glm::translate(glm::identity<glm::mat4>(), transform.translation) * glm::toMat4(transform.rotation);
    }
}

} // namespace reach::transform_system

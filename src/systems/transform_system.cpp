#include <components/transform_component.hpp>
#include <world.hpp>

namespace reach::transform_system {

void update() {
    for (auto [entity, transform] : World::current().view<TransformComponent>().each()) {
        transform.model = glm::translate(glm::identity<glm::mat4>(), transform.translation) * glm::toMat4(transform.rotation);

        TransformParent *parent = World::current().try_get<TransformParent>(entity);

        if (parent != nullptr) {
            transform.world = World::current().get<TransformComponent>(parent->id).world * transform.model;
        } else {
            transform.world = transform.model;
        }
    }
}

} // namespace reach::transform_system

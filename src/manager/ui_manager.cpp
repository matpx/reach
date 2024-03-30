#include <components/camera_component.hpp>
#include <data/mesh_data.hpp>
#include <manager/device_manager.hpp>
#include <manager/material_manager.hpp>
#include <manager/ui_manager.hpp>
#include <utils/conditions.hpp>
#include <world.hpp>

namespace reach {

static UiManager *self = nullptr;

UiManager &UiManager::get() { return *self; }

UiManager::UiManager() {
    PRECONDITION(self == nullptr);

    self = this;
}

UiManager::~UiManager() {
    PRECONDITION(self != nullptr);

    self = nullptr;
}

void UiManager::draw_rect(const glm::vec2 &min, const glm::vec2 &max) {
    vertex_data.push_back(BasicVertex{.position = {max.x, min.y, 0}});
    vertex_data.push_back(BasicVertex{.position = {min.x, min.y, 0}});
    vertex_data.push_back(BasicVertex{.position = {min.x, max.y, 0}});

    vertex_data.push_back(BasicVertex{.position = {max.x, min.y, 0}});
    vertex_data.push_back(BasicVertex{.position = {min.x, max.y, 0}});
    vertex_data.push_back(BasicVertex{.position = {max.x, max.y, 0}});
}

void UiManager::submit_data() {
    if (vertex_data.size() == 0) {
        return;
    }

    const auto &world = World::current();

    DeviceManager::get().draw_immediate(world.get<CameraComponent>(world.current_camera).ortho, vertex_data,
                                        MaterialManager::get().immediate_material);

    vertex_data.resize(0);
}

} // namespace reach

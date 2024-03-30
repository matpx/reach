#pragma once

#include <components/material_component.hpp>
#include <data/mesh_data.hpp>
#include <manager/manager.hpp>
#include <vec2.hpp>

namespace reach {

class UiManager final : public Manager {
    private:
        std::vector<VertexWithPosition> vertex_data;

        MaterialComponent immediate_material = {};

    public:
        [[nodiscard]] static UiManager &get();

        UiManager();
        ~UiManager();

        constexpr void draw_rect(const glm::vec2 &min, const glm::vec2 &max) {
            vertex_data.push_back(VertexWithPosition{.position = {max.x, min.y, 0}});
            vertex_data.push_back(VertexWithPosition{.position = {min.x, min.y, 0}});
            vertex_data.push_back(VertexWithPosition{.position = {min.x, max.y, 0}});

            vertex_data.push_back(VertexWithPosition{.position = {max.x, min.y, 0}});
            vertex_data.push_back(VertexWithPosition{.position = {min.x, max.y, 0}});
            vertex_data.push_back(VertexWithPosition{.position = {max.x, max.y, 0}});
        }

        void submit_to_device();
};

} // namespace reach
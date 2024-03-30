#pragma once

#include <components/material_component.hpp>
#include <data/mesh_data.hpp>
#include <manager/manager.hpp>
#include <vec2.hpp>

namespace reach {

class UiManager final : public Manager {
    private:
        std::vector<Vertex2D> immediate_data;

        MaterialComponent immediate_material = {};

    public:
        [[nodiscard]] static UiManager &get();

        UiManager();
        ~UiManager();

        constexpr void draw_rect(const glm::vec2 &min, const glm::vec2 &max) {
            const std::array<Vertex2D, 6> rect = {
                Vertex2D{{max.x, min.y}}, Vertex2D{{min.x, min.y}}, Vertex2D{{min.x, max.y}},
                Vertex2D{{max.x, min.y}}, Vertex2D{{min.x, max.y}}, Vertex2D{{max.x, max.y}},
            };

            immediate_data.insert(immediate_data.end(), std::begin(rect), std::end(rect));
        }

        void submit_to_device();
};

} // namespace reach
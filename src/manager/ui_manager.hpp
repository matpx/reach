#pragma once

#include <components/material_component.hpp>
#include <data/mesh_data.hpp>
#include <ext/scalar_constants.hpp>
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

        constexpr void draw_circle(const glm::vec2 &center, float radius) {
            constexpr int32_t segment_count = 32;
            constexpr float segment_radius = 1.0f / segment_count * 2.0f * glm::pi<float>();

            float corner_a = 0.0f;

            for (int32_t i_segment = 1; i_segment <= segment_count; i_segment++) {
                const float corner_b = corner_a + segment_radius;

                immediate_data.push_back(Vertex2D::from(center + glm::vec2{sinf(corner_a), cosf(corner_a)} * radius));
                immediate_data.push_back(Vertex2D::from(center + glm::vec2{sinf(corner_b), cosf(corner_b)} * radius));
                immediate_data.push_back(Vertex2D::from(center));

                corner_a = corner_b;
            }
        }

        void submit_to_device();
};

} // namespace reach
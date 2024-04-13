#pragma once

#include <array>
#include <nvrhi/nvrhi.h>
#include <stdint.h>
#include <string_view>
#include <utils/nocopy.hpp>
#include <vec2.hpp>
#include <vec4.hpp>
#include <vector>

namespace reach {

using MeshIndex = uint32_t;

struct Vertex2D {
        std::array<float, 2> position;
        std::array<float, 4> color;

        constexpr static Vertex2D from(const glm::vec2 &position_vec, const glm::vec4 &color_vec) {
            return {
                .position = {position_vec.x, position_vec.y},
                .color = {color_vec.x, color_vec.y, color_vec.z, color_vec.w},
            };
        }
};

struct Vertex3D {
        std::array<float, 3> position;
        std::array<float, 3> normal;
};

struct MeshData {
        std::string_view debug_name = "<unnamed MeshData>";

        std::vector<Vertex3D> vertex_data;
        std::vector<MeshIndex> index_data;

        nvrhi::BufferHandle vertex_buffer;
        nvrhi::BufferHandle index_buffer;

        [[nodiscard]] bool is_uploaded() { return vertex_buffer != nullptr; }
};

} // namespace reach

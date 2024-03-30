#pragma once

#include <array>
#include <sokol_gfx.h>
#include <stdint.h>
#include <string_view>
#include <vector>

namespace reach {

struct VertexWithPosition {
        std::array<float, 3> position;
};

struct VertexWithPositionNormal {
        std::array<float, 3> position;
        std::array<float, 3> normal;
};

using MeshIndex = uint32_t;

struct MeshData {
        std::string_view debug_name = "<unnamed MeshData>";

        std::vector<VertexWithPositionNormal> vertex_data;
        std::vector<MeshIndex> index_data;

        sg_buffer vertex_buffer = {};
        sg_buffer index_buffer = {};

        [[nodiscard]] constexpr bool is_uploaded() { return vertex_buffer.id != SG_INVALID_ID; }
};

} // namespace reach

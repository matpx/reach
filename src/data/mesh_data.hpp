#pragma once

#include <array>
#include <stdint.h>
#include <vector>

namespace reach {

struct MeshVertex {
        std::array<float, 3> pos;
};

using MeshIndex = uint32_t;

struct MeshData {
        std::vector<MeshVertex> vertex_data;
        std::vector<MeshIndex> index_data;
};

} // namespace reach

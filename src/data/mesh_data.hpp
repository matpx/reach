#pragma once

#include <stdint.h>
#include <vector>

namespace reach {

struct MeshVertex {
        float pos[3];
};

using MeshIndex = uint32_t;

struct MeshData {
        std::vector<MeshVertex> vertex_data;
        std::vector<MeshIndex> index_data;
};

} // namespace reach

#pragma once

#include <components/material_component.hpp>
#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <optional>

namespace reach {

struct Prefab {
        struct Node {
                std::optional<MeshComponent> mesh;
                std::optional<MaterialComponent> material;
                TransformComponent transform;
        };

        std::vector<Node> nodes;
};

} // namespace reach

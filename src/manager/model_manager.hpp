#pragma once

#include <components/material_component.hpp>
#include <components/mesh_component.hpp>
#include <components/transform_component.hpp>
#include <manager/manager.hpp>
#include <optional>
#include <tl/expected.hpp>


namespace reach {

class World;

struct Prefab {
        struct Node {
                std::optional<MeshComponent> mesh;
                std::optional<MaterialComponent> material;
                TransformComponent transform;
        };

        std::vector<Node> nodes;
};

class ModelManager final : public Manager {
    public:
        [[nodiscard]] static ModelManager &get();

        ModelManager();
        ~ModelManager();

        [[nodiscard]] tl::expected<std::shared_ptr<Prefab>, std::string> load_gltf(const std::string_view path);

        void instantiate(World &world, const std::shared_ptr<Prefab> prefab);
};

} // namespace reach
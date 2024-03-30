#pragma once

#include <manager/manager.hpp>
#include <memory>
#include <string_view>
#include <tl/expected.hpp>


namespace reach {

class World;
struct Prefab;

class ModelManager final : public Manager {
    public:
        [[nodiscard]] static ModelManager &get();

        ModelManager();
        ~ModelManager();

        [[nodiscard]] tl::expected<std::shared_ptr<Prefab>, std::string> load_gltf(const std::string_view path);

        void instantiate(World &world, const std::shared_ptr<Prefab> prefab);
};

} // namespace reach
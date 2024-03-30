#pragma once

#include <data/mesh_data.hpp>
#include <manager/manager.hpp>
#include <vec2.hpp>

namespace reach {

class UiManager final : public Manager {
    private:
        std::vector<BasicVertex> vertex_data;

    public:
        [[nodiscard]] static UiManager &get();

        UiManager();
        ~UiManager();

        void draw_rect(const glm::vec2 &min, const glm::vec2 &max);

        void submit_data();
};

} // namespace reach
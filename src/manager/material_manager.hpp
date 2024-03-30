#pragma once

#include <components/material_component.hpp>
#include <manager/manager.hpp>

namespace reach {

class MaterialManager final : public Manager {
    public:
        [[nodiscard]] static MaterialManager &get();

        MaterialManager();
        ~MaterialManager();

        MaterialComponent unlit_material = {};
        MaterialComponent immediate_material = {};
};

} // namespace reach
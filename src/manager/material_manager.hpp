#pragma once

#include <components/material_component.hpp>
#include <manager/manager.hpp>

namespace reach {

class MaterialManager final : public Manager {
    public:
        static MaterialManager &get();

        MaterialManager();
        ~MaterialManager();

        MaterialComponent unlit_material = {};
};

} // namespace reach
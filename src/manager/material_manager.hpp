#pragma once

#include "components/material_component.hpp"
#include "nocopy.hpp"


namespace reach {

class MaterialManager : NoCopy {
    public:
        MaterialManager();
        ~MaterialManager();

        MaterialComponent unlit_material = {};
};

} // namespace reach
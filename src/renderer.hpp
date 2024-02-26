#pragma once

#include "nocopy.hpp"
#include "sokol_gfx.h"

namespace reach {

class Renderer : NoCopy {
    public:
        Renderer();
        ~Renderer();

        sg_pipeline unlit_pipeline;
};

} // namespace reach
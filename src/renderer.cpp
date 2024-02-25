#include "renderer.hpp"
#include "log.hpp"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "src/renderer.hpp"

namespace reach {

Renderer::Renderer() {
    sg_setup(sg_desc{.logger = {.func = slog_func}});
    if (!sg_isvalid()) {
        log_fatal_and_abort("sg_setup() failed");
    }
}

Renderer::~Renderer() { sg_shutdown(); }

} // namespace reach

#include "device_manager.hpp"
#include "log.hpp"
#include "sokol_gfx.h"

namespace reach {

void sokol_log([[maybe_unused]] const char *tag, [[maybe_unused]] uint32_t log_level,
               [[maybe_unused]] uint32_t log_item, const char *message, [[maybe_unused]] uint32_t line_nr,
               [[maybe_unused]] const char *filename, [[maybe_unused]] void *user_data) {
    log_error(message);
}

DeviceManager::DeviceManager() {
    sg_setup(sg_desc{.logger = {.func = sokol_log}});
    if (!sg_isvalid()) {
        panic("sg_setup() failed");
    }
}

DeviceManager::~DeviceManager() { sg_shutdown(); }

} // namespace reach

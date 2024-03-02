#pragma once

#include <utils/log.hpp>

namespace reach {

#define PANIC(...)                                                                                                     \
    LOG_ERROR(__VA_ARGS__);                                                                                            \
    LOG_ERROR("aborting!");                                                                                            \
    std::abort();

} // namespace reach
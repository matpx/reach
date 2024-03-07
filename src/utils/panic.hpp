#pragma once

#include <utils/log.hpp>

#define PANIC(...)                                                                                 \
    LOG_ERROR(__VA_ARGS__);                                                                        \
    LOG_ERROR("Terminating after PANIC() was called!");                                            \
    std::terminate();

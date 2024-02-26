#pragma once

#include <cstdio>
#include <string_view>

namespace reach {

inline void log_error(const std::string_view message) { printf("[ERROR]: %s\n", message.data()); }

inline void log_fatal(const std::string_view message) { printf("[FATAL]: %s\n", message.data()); }

inline void panic(const std::string_view message) {
    log_fatal(message);
    std::abort();
}

} // namespace reach

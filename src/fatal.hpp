#pragma once

#include <cstdio>
#include <string>

inline void log_error(const std::string &message) { printf("[ERROR]: %s\n", message.c_str()); }

inline void fatal(const std::string &message) {
    printf("[FATAL]: %s\n", message.c_str());
    std::abort();
}

#pragma once

#include "log.hpp"

namespace reach {

template <typename... T> inline void panic(T... args) {
    log_error(std::forward<T>(args)...);
    std::abort();
}

} // namespace reach
#pragma once

#include <spdlog/spdlog.h>

namespace reach {

template <typename... T> constexpr void log_debug(T... args) { spdlog::debug(std::forward<T>(args)...); }
template <typename... T> constexpr void log_info(T... args) { spdlog::info(std::forward<T>(args)...); }
template <typename... T> constexpr void log_warning(T... args) { spdlog::warn(std::forward<T>(args)...); }
template <typename... T> constexpr void log_error(T... args) { spdlog::error(std::forward<T>(args)...); }

} // namespace reach

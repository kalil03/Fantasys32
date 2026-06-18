#pragma once

#include <string_view>

template <typename... ValidStrings>
constexpr bool is_str_one_of(std::string_view target, ValidStrings... valid_strings) {
    // This expands at compile-time to: (target == v1) || (target == v2) || ...
    return ((target == valid_strings) || ...);
}

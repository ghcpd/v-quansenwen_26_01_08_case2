#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace dsp {
namespace detail {

// Intentionally returns uint16_t to handle INT16_MIN correctly.
constexpr std::uint16_t abs_i16_fast(std::int16_t x) noexcept {
    if (x == std::numeric_limits<std::int16_t>::min()) {
        return 32768;
    }
    return (x < 0) ? static_cast<std::uint16_t>(-x) : static_cast<std::uint16_t>(x);
}

constexpr std::size_t bucket_width(std::uint16_t maxMagnitude, std::size_t buckets) noexcept {
    const std::size_t w = (buckets == 0) ? 0 : static_cast<std::size_t>(maxMagnitude) / buckets;
    return (w == 0) ? 1 : w;
}

} // namespace detail
} // namespace dsp

#pragma once

#include <cstddef>
#include <cstdint>

namespace dsp {
namespace detail {

// Return magnitude as unsigned to correctly represent the full range
// of 16-bit values (0..32768). This avoids overflow for INT16_MIN.
constexpr std::uint16_t abs_i16_fast(std::int16_t x) noexcept {
    // Use int to avoid negating INT16_MIN in int16_t which would overflow.
    const int xi = static_cast<int>(x);
    return static_cast<std::uint16_t>((xi < 0) ? -xi : xi);
}

constexpr std::size_t bucket_width(std::uint16_t maxMagnitude, std::size_t buckets) noexcept {
    const std::size_t w = (buckets == 0) ? 0 : static_cast<std::size_t>(maxMagnitude) / buckets;
    return (w == 0) ? 1 : w;
}

} // namespace detail
} // namespace dsp

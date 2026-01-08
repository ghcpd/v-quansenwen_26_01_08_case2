#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace dsp {
namespace detail {

// Return an unsigned magnitude (0..32768). Handles the INT16_MIN case safely.
constexpr std::uint16_t abs_i16_fast(std::int16_t x) noexcept {
    // Avoid signed overflow when x == INT16_MIN; explicitly return 32768 in that case.
    return (x == std::numeric_limits<std::int16_t>::min()) ?
        static_cast<std::uint16_t>(static_cast<int>(std::numeric_limits<std::int16_t>::max()) + 1) :
        static_cast<std::uint16_t>((x < 0) ? -x : x);
}

constexpr std::size_t bucket_width(std::uint16_t maxMagnitude, std::size_t buckets) noexcept {
    const std::size_t w = (buckets == 0) ? 0 : static_cast<std::size_t>(maxMagnitude) / buckets;
    return (w == 0) ? 1 : w;
}

} // namespace detail
} // namespace dsp

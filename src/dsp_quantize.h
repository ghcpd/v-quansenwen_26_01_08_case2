#pragma once

#include <cstddef>
#include <cstdint>

namespace dsp {
namespace detail {

// Returns magnitude as unsigned to avoid sign/overflow issues with INT16_MIN.
constexpr std::uint16_t abs_i16_fast(std::int16_t x) noexcept {
    // Use a wider signed type to avoid UB when negating INT16_MIN.
    return (x < 0) ? static_cast<std::uint16_t>(-static_cast<std::int32_t>(x)) : static_cast<std::uint16_t>(x);
}

constexpr std::size_t bucket_width(std::uint16_t maxMagnitude, std::size_t buckets) noexcept {
    const std::size_t w = (buckets == 0) ? 0 : static_cast<std::size_t>(maxMagnitude) / buckets;
    return (w == 0) ? 1 : w;
}

} // namespace detail
} // namespace dsp

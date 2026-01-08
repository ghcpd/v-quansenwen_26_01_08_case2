#include "audio_histogram.h"

#include "dsp_quantize.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <stdexcept>

AudioHistogram::AudioHistogram(std::size_t buckets, std::uint16_t maxMagnitude)
    : AudioHistogram(Config{buckets, maxMagnitude}) {
}

AudioHistogram::AudioHistogram(Config cfg)
    : m_counts(cfg.buckets, 0), m_cfg(cfg) {
    if (m_cfg.buckets == 0) {
        throw std::invalid_argument("buckets must be > 0");
    }
    if (m_cfg.dcSmoothingShift > 15) {
        throw std::invalid_argument("dcSmoothingShift must be in [0, 15]");
    }
}

void AudioHistogram::addSample(std::int16_t sample) {
    m_stats.samplesSeen += 1;

    const std::int16_t processed = preprocess(sample);
    const std::size_t bucket = bucketFor(processed);
    m_counts.at(bucket) += 1;
    m_stats.samplesCounted += 1;
}

void AudioHistogram::addSamples(const std::vector<std::int16_t>& samples) {
    for (auto s : samples) {
        addSample(s);
    }
}

std::int16_t AudioHistogram::preprocess(std::int16_t sample) {
    // Track clipping in the raw stream.
    if (sample == std::numeric_limits<std::int16_t>::min() ||
        sample == std::numeric_limits<std::int16_t>::max()) {
        m_stats.clipped += 1;
    }

    std::int32_t x = static_cast<std::int32_t>(sample);

    if (m_cfg.enableDcBlock) {
        // Simple leaky integrator DC estimator.
        const std::int32_t est = m_stats.dcEstimateQ0;
        const std::int32_t delta = x - est;
        m_stats.dcEstimateQ0 = est + (delta >> m_cfg.dcSmoothingShift);
        x = x - m_stats.dcEstimateQ0;
    }

    // Clamp back to int16 after processing.
    x = std::clamp<std::int32_t>(x,
        static_cast<std::int32_t>(std::numeric_limits<std::int16_t>::min()),
        static_cast<std::int32_t>(std::numeric_limits<std::int16_t>::max()));

    const std::int16_t y = static_cast<std::int16_t>(x);

    // Optional noise gate based on magnitude.
    if (m_cfg.noiseGate > 0) {
        const std::uint16_t mag = dsp::detail::abs_i16_fast(y);
        if (mag < m_cfg.noiseGate) {
            return 0;
        }
    }

    return y;
}

std::size_t AudioHistogram::bucketFor(std::int16_t sample) const {
    const std::uint16_t magnitude = dsp::detail::abs_i16_fast(sample);
    const std::size_t bucketWidth = std::max<std::size_t>(
        1, dsp::detail::bucket_width(m_cfg.maxMagnitude, m_counts.size()));

    const std::size_t bucket = static_cast<std::size_t>(magnitude) / bucketWidth;
    return std::min(bucket, m_counts.size() - 1);
}

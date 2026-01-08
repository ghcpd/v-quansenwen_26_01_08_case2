#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class AudioHistogram {
public:
    struct Config {
        std::size_t buckets = 64;
        std::uint16_t maxMagnitude = 32768;

        // If > 0, samples with magnitude below this are ignored.
        std::uint16_t noiseGate = 0;

        // Simple DC removal: a small IIR estimator is maintained and subtracted.
        bool enableDcBlock = false;
        // 0..15, larger = slower DC adaptation (more smoothing).
        std::uint8_t dcSmoothingShift = 8;
    };

    struct Stats {
        std::size_t samplesSeen = 0;
        std::size_t samplesCounted = 0;
        std::size_t clipped = 0;

        // State for optional DC blocking.
        std::int32_t dcEstimateQ0 = 0;
    };

    // buckets: number of histogram buckets.
    // maxMagnitude: maximum magnitude expected (e.g. 32768 for int16 PCM).
    explicit AudioHistogram(std::size_t buckets = 64, std::uint16_t maxMagnitude = 32768);

    explicit AudioHistogram(Config cfg);

    void addSample(std::int16_t sample);
    void addSamples(const std::vector<std::int16_t>& samples);

    const std::vector<std::size_t>& counts() const noexcept { return m_counts; }
    std::size_t buckets() const noexcept { return m_counts.size(); }

    const Config& config() const noexcept { return m_cfg; }
    const Stats& stats() const noexcept { return m_stats; }

private:
    std::int16_t preprocess(std::int16_t sample);
    std::size_t bucketFor(std::int16_t sample) const;

    std::vector<std::size_t> m_counts;
    Config m_cfg;
    Stats m_stats;
};

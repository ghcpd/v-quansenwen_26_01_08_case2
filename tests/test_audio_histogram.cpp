#include "audio_histogram.h"

#include "doctest/doctest.h"

#include <cstdint>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

static std::vector<std::int16_t> readSamplesFile(const std::string& path) {
    std::ifstream in(path);
    std::vector<std::int16_t> out;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        const int v = std::stoi(line);
        out.push_back(static_cast<std::int16_t>(v));
    }
    return out;
}

static std::size_t expectedBucketSafe(std::int16_t sample, std::size_t buckets, std::uint16_t maxMagnitude = 32768) {
    const int s = static_cast<int>(sample);
    const int mag = (s == std::numeric_limits<std::int16_t>::min()) ? 32768 : std::abs(s);
    const std::size_t bucketWidth = std::max<std::size_t>(1, static_cast<std::size_t>(maxMagnitude / buckets));
    const std::size_t bucket = static_cast<std::size_t>(mag) / bucketWidth;
    return std::min(bucket, buckets - 1);
}

TEST_CASE("AudioHistogram: INT16_MIN should not throw") {
    AudioHistogram hist(64);
    const auto minSample = std::numeric_limits<std::int16_t>::min();

    CHECK_NOTHROW(hist.addSample(minSample));
}

TEST_CASE("AudioHistogram: file-driven load with INT16_MIN should not throw and should increment expected bucket") {
    AudioHistogram hist(64);
    const auto samples = readSamplesFile("../../data/pcm_samples.txt");
    const std::size_t expected = expectedBucketSafe(std::numeric_limits<std::int16_t>::min(), hist.buckets());

    REQUIRE_NOTHROW(hist.addSamples(samples));
    CHECK_EQ(hist.counts().at(expected), 1u);
}

int main() {
    doctest::Context ctx;
    return ctx.run();
}

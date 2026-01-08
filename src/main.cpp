#include "audio_histogram.h"

#include <cstdint>
#include <iostream>
#include <vector>

int main() {
    AudioHistogram::Config cfg;
    cfg.buckets = 64;
    cfg.enableDcBlock = true;
    cfg.dcSmoothingShift = 9;
    cfg.noiseGate = 4;
    AudioHistogram hist(cfg);

    std::vector<std::int16_t> samples = {0, -1, 123, -1000, 2000};
    hist.addSamples(samples);

    std::cout << "Buckets: " << hist.buckets() << "\n";
    std::cout << "First bucket count: " << hist.counts().at(0) << "\n";
    std::cout << "Samples seen: " << hist.stats().samplesSeen << "\n";
    std::cout << "Samples counted: " << hist.stats().samplesCounted << "\n";
    std::cout << "Clipped samples: " << hist.stats().clipped << "\n";

    return 0;
}

#include "src/audio_histogram.h"
#include <iostream>
#include <fstream>

int main() {
    AudioHistogram h(64);
    const auto samples = [] {
        std::ifstream in("data/pcm_samples.txt");
        std::vector<std::int16_t> out;
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty()) continue;
            const int v = std::stoi(line);
            out.push_back(static_cast<std::int16_t>(v));
        }
        return out;
    }();

    for (auto s : samples) {
        std::cout << "sample=" << s << "\n";
        h.addSample(s);
        std::cout << "  mapped bucket: ";
        try {
            for (std::size_t i = 0; i < h.counts().size(); ++i) {
                if (h.counts().at(i) != 0) std::cout << i << " ";
            }
            std::cout << "\n";
        } catch (const std::exception& e) {
            std::cout << "exception: " << e.what() << "\n";
        }
    }
    std::cout << "final counts size=" << h.counts().size() << " buckets()=" << h.buckets() << "\n";
    for (std::size_t i = 0; i < h.counts().size(); ++i) std::cout << i <<":"<< h.counts().at(i) <<"\n";
    return 0;
}

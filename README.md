# runtime-error-0105

A small, near-real-world C++ module that builds an amplitude histogram from 16-bit PCM audio samples.

This repo contained a **reproducible runtime issue** that has been fixed:
certain extreme input values caused a **runtime `std::out_of_range` exception** while building the histogram.

## Project structure

```
.
├─ CMakeLists.txt
├─ src/
│  ├─ audio_histogram.h
│  ├─ audio_histogram.cpp
│  └─ main.cpp
├─ tests/
│  ├─ CMakeLists.txt
│  └─ test_audio_histogram.cpp
├─ third_party/
│  └─ doctest/
│     └─ doctest.h
└─ data/
   └─ pcm_samples.txt
```

## Build + run tests (single command)

From the repo root:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build --config Debug && ctest --test-dir build -C Debug --output-on-failure
```

Expected: tests **pass** after fixing the runtime issue.

## What this demonstrates

- A realistic data-processing module (audio amplitude histogram)
- A simple but non-trivial runtime failure scenario
- Clear tests that validate the fix

See `KNOWN_ISSUE.md` for details on the original issue.

# Resolved Issue: runtime out_of_range while building histogram

## Issue type

Runtime error: `std::out_of_range` thrown during histogram updates. **RESOLVED**

## Trigger conditions

When processing certain extreme 16-bit PCM inputs (rare edge values), histogram building could throw. **FIXED**

## Expected vs actual

- Expected: histogram building should not throw for any valid 16-bit PCM sample.
- Actual: Previously, `AudioHistogram::addSample(-32768)` could throw `std::out_of_range`. Now fixed.

## Where it happens

- Public API: `AudioHistogram::addSample(std::int16_t)`
- File: `src/audio_histogram.cpp`

## Fix summary

- Corrected absolute value calculation for INT16_MIN to return 32768.
- Added clamping in bucket calculation to prevent out-of-range access.
- Updated test helper to match implementation.

## Reproduction

Run the automated tests; they now pass:

- A direct unit test calling `addSample(-32768)`.
- A small file-driven test that reads `data/pcm_samples.txt` containing `-32768`.

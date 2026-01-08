# Known Issue: runtime out_of_range while building histogram

## Issue type

Runtime error: `std::out_of_range` thrown during histogram updates.

## Trigger conditions

When processing certain extreme 16-bit PCM inputs (rare edge values), histogram building can throw.

## Expected vs actual

- Expected: histogram building should not throw for any valid 16-bit PCM sample.
- Actual: `AudioHistogram::addSample(-32768)` can throw `std::out_of_range`.

## Where it happens

- Public API: `AudioHistogram::addSample(std::int16_t)`
- File: `src/audio_histogram.cpp`

## Reproduction

Run the automated tests; they include:

- A direct unit test calling `addSample(-32768)`.
- A small file-driven test that reads `data/pcm_samples.txt` containing `-32768`.

## Status: Fixed

This issue has been fixed. The fix ensures magnitudes are computed without overflow for
`INT16_MIN` and clamps bucket indices so the maximum magnitude maps to the last bucket.

To verify locally:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && \
	cmake --build build --config Debug && \
	ctest --test-dir build -C Debug --output-on-failure
```

All tests should pass (100% tests passed).

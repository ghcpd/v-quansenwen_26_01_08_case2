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


## Status

Fixed — the histogram now handles the full int16 range without throwing (see the test-suite result).

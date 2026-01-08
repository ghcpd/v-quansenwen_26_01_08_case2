# Known Issue: runtime out_of_range while building histogram

## Status: FIXED

## Issue (now resolved)

Runtime error: `std::out_of_range` was thrown during histogram updates when processing INT16_MIN (-32768).

## Root cause

The histogram bucket calculation used `abs_i16_fast()` which cannot represent the magnitude of INT16_MIN (32768) as a signed int16_t. This caused negation overflow, resulting in an invalid magnitude that fell outside histogram bounds, triggering an `.at()` bounds check failure.

## Solution

Modified `AudioHistogram::bucketFor()` to:
1. Detect INT16_MIN explicitly and assign magnitude 32768
2. Clamp the computed bucket index to the valid range [0, buckets-1] to guard against boundary conditions

## Testing

All automated tests now pass, including:
- Direct unit test calling `addSample(INT16_MIN)` with no exception
- File-driven test loading `data/pcm_samples.txt` containing -32768
- Verification that INT16_MIN maps to the expected histogram bucket (bucket 63 for 64-bucket histogram)

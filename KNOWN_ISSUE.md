# Known Issue: runtime out_of_range while building histogram (resolved)

## Issue type

Runtime error: `std::out_of_range` thrown during histogram updates (fixed).

## Summary of the fix

- The absolute-magnitude helper now returns an unsigned magnitude in the full range 0..32768 and handles `INT16_MIN` safely.
- The histogram bucket computation clamps the top value so the maximum magnitude maps to the last bucket (no out-of-range index).

## Verification

Build and run the tests:

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build --config Debug && ctest --test-dir build -C Debug --output-on-failure

The unit tests now pass.

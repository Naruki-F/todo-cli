# Tests

This folder contains plain C++ `assert` tests and the test runner for the console to-do list MVP.

Run all automated checks from the repository root:

```bash
./tests/run_tests.sh
```

The runner compiles test binaries into the ignored `build/` directory. Keep test source in this folder so it is not compiled into the interactive application binary.

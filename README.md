# C++ Console To-Do List

A simple session-only console application for adding, listing, completing, and deleting tasks. Tasks are kept in memory and are lost when the program exits.

## Getting Started

This repository is compatible with [cpp-container](https://github.com/ChicoState/cpp-container). If not already built on your machine, clone and build it.

Run the container:

```bash
docker run -v "$(pwd)":/usr/src -it cpp-container
```

Run the application interactively in a shell:

```bash
docker run -v "$(pwd)":/usr/src -it cpp-container sh
```

From the project directory inside the container (or from any local environment with a C++17 compiler), build and run the application:

```bash
./test_runner.sh
```

The menu provides these actions:

- `1` — Add task
- `2` — List tasks
- `3` — Mark task done
- `4` — Delete task
- `5` — Quit

Task titles have leading and trailing whitespace removed. Empty titles are rejected. A completed task remains visible until deleted.

## Testing

Run all automated tests with:

```bash
./tests/run_tests.sh
```

The test runner builds temporary binaries in the ignored `build/` directory, runs assert-based domain tests, and verifies a scripted console session.

## Structure

* `.agents` - AI agent configurations and skills (in `/skills` subdirectory) for this project
* `.` - The root directory contains the C++ code for the application as well as necessary scripts
* `specs` - Specification and implementation-plan documentation
* `tests` - Assert-based unit tests and the automated test runner

# Spec: Console To-Do List MVP

## Objective

Build a simple C++ console application that lets one user manage a short to-do list during a single program session. The user can add a task, list tasks, mark a task done, delete a task, and quit through a fixed numbered menu. Success means that each action is usable, invalid input is handled without ending the session, and automated checks prove the domain rules and a representative console flow.

This specification is the source of truth for behavior. The ordered delivery work remains in [todo-list-mvp.md](todo-list-mvp.md).

## Tech Stack

- C++17.
- C++ standard library only; no third-party runtime or test dependencies.
- `g++` compiler, with `-Wall -Wextra -pedantic` warnings enabled.
- Plain C++ `assert` tests and a shell-scripted console integration test.
- In-memory task storage only; all task data is discarded when the process exits.

## Commands

Build and run the interactive application:

```sh
./test_runner.sh
```

Run all automated checks:

```sh
./tests/run_tests.sh
```

The application runner must compile the root application sources as C++17 and write its executable to `app`. The test runner must begin with `mkdir -p build`, compile its test and application binaries under `build/`, and stop on failures with `set -e`.

## Project Structure

```text
main.cpp                    → application entry point
todo.h / todo.cpp           → Task model and TodoList business rules
console_app.h / console_app.cpp → menu loop, parsing, and user-facing output
test_runner.sh              → build and run the interactive application
tests/todo_list_tests.cpp   → plain assert-based domain tests
tests/run_tests.sh          → build and run unit and console integration checks
specs/todo-list-mvp-spec.md → behavior source of truth
specs/todo-list-mvp.md      → ordered implementation plan
.gitignore                  → ignores app and build artifacts
```

The production source files stay at the repository root, matching the project's existing lightweight layout. Test source stays under `tests/` and must not be compiled into the application binary.

## Code Style

Use focused classes with clear ownership: `TodoList` owns task state and mutations, while `ConsoleApp` owns prompts, parsing, and output. Use PascalCase for types, camelCase for functions and variables, and `const` for values that are not modified. Read every terminal response with `std::getline`, then parse and validate it deliberately.

```cpp
bool TodoList::markDone(std::size_t id) {
    for (Task& task : tasks_) {
        if (task.id == id && !task.done) {
            task.done = true;
            return true;
        }
    }
    return false;
}
```

Keep functions small and return an explicit success/failure result for operations that can reject input or an unknown ID. Do not introduce abstractions or dependencies beyond what this MVP needs.

## Testing Strategy

Use `assert` statements in `tests/todo_list_tests.cpp` to verify domain behavior independently of terminal I/O:

- valid task creation;
- trimming leading and trailing title whitespace;
- rejection of empty and whitespace-only titles;
- unique, increasing IDs that are not reused after deletion;
- marking a known pending task done;
- rejection of unknown or already-done IDs; and
- deletion without affecting unrelated tasks.

Use `tests/run_tests.sh` for a non-interactive console test. Pipe the fixed menu sequence into the app and check stable output fragments for add, list, done, delete, invalid input recovery, and quit. The script must return nonzero when a build, assertion, or output check fails.

## Boundaries

- Always: compile as C++17 with warnings enabled; validate and trim title input; use line-based input; run `./tests/run_tests.sh` before declaring implementation complete; keep generated binaries out of version control.
- Ask first: add persistence or a database; add third-party dependencies or a testing framework; change the fixed menu contract; add task editing, priorities, dates, categories, search, accounts, or CI configuration.
- Never: commit secrets or generated binaries; silently accept blank task titles; reuse deleted task IDs; terminate the menu loop on ordinary invalid input; compile test source into the application executable.

## Success Criteria

- The program provides this explicit menu: `1 = Add task`, `2 = List tasks`, `3 = Mark task done`, `4 = Delete task`, and `5 = Quit`.
- A valid task title is stored after leading and trailing whitespace are trimmed; empty or whitespace-only titles are rejected.
- Every task has a unique increasing numeric ID; deleted IDs are never reused.
- A pending task can be marked done once and remains visible until it is deleted.
- Listing makes each task's ID, title, and pending/done state clear.
- Invalid menu selections, malformed IDs, unknown IDs, blank titles, and repeated mark-done attempts show feedback and keep the application usable.
- EOF from redirected input exits gracefully.
- Task data is session-only and is lost on exit.
- `test_runner.sh` builds and starts the interactive application, and its `app` output is ignored by Git.
- `tests/run_tests.sh` creates `build/` and passes both assert-based and scripted console tests.

## Open Questions

None for the MVP. Completion is intentionally one-way, title trimming is required, persistence is excluded, and tests use plain `assert` statements. Human approval of this specification and the existing implementation plan is required before Phase 4 implementation begins.

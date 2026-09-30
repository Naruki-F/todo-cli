# Feature Description

Create a simple C++ console to-do list application. During one program session, users can add tasks, list them, mark tasks as done, delete tasks, and quit through a numbered text menu.

The MVP intentionally does not persist tasks to disk and does not include editing, priorities, due dates, categories, accounts, or search.

# User Story

As a console user, I want to add and manage to-do items through a simple menu so that I can track tasks during my current program session.

# Problem Statement

The repository is a minimal C++ template: `main.cpp` exits immediately, there is no domain model or application behavior, and no automated test harness exists. Users therefore cannot currently manage to-do items.

# Solution Statement

Implement a small standard-library-only application with separate domain and console-I/O responsibilities:

- `TodoList` owns task storage and business rules.
- `Task` contains a monotonically generated numeric ID, trimmed title, and `done` status.
- `ConsoleApp` renders a menu, accepts line-based input, displays results, and invokes `TodoList`.
- Tests use plain C++ `assert` statements and a shell script to validate both domain behavior and a scripted console session.
- Task data remains in memory only and is discarded when the program exits.

# Relevant Files

### Existing Files

- `main.cpp` — replace the placeholder entry point with application startup.
- `README.md` — retain container instructions and document how to run and test the MVP.
- `test_runner.sh` — preserve its purpose as the application build-and-run command; update it only as necessary for the new source layout and C++17 compilation.
- `.gitignore` — add the root-level `app` binary produced by `test_runner.sh`, because the existing `*.app` pattern does not match it.
- `tests/README.md` — optionally update with the test command and test-file convention.
- `specs/README.md` — existing location guidance for this specification.

### New Files

- `todo.h` — declare `Task` and `TodoList`.
- `todo.cpp` — implement validation, ID creation, task completion, deletion, and read-only listing.
- `console_app.h` — declare a testable console application interface.
- `console_app.cpp` — implement menu rendering, line input parsing, and user feedback.
- `tests/todo_list_tests.cpp` — plain assert-based domain tests.
- `tests/run_tests.sh` — build and run the unit tests and scripted console integration test.

# Implementation Plan

### Phase 1: Domain Foundation

Define task state and implement the in-memory `TodoList` independently from terminal input. This establishes testable business rules before user-interface work begins.

### Phase 2: Console Interaction

Build the numbered menu and connect each command to the domain layer. Use `std::getline` for every prompt so invalid or mixed input cannot leave the stream in a failed state.

### Phase 3: Automated Verification and Documentation

Add a separate automated test script, retain `test_runner.sh` as the interactive app launcher, and document the supported workflow and session-only limitation.

# Step by Step Tasks

### Task 1: Define the task-management interface and initial unit tests

Create the `Task` data structure and `TodoList` public interface. Write initial assert-based tests that describe valid adds, title validation, completion, deletion, and stable IDs before implementing the methods.

**Dependencies:** None

**Acceptance criteria:**

- [ ] A task exposes an ID, a title, and a done/not-done status.
- [ ] The public API supports add, list, mark-done-by-ID, and delete-by-ID operations.
- [ ] Tests specify that leading/trailing whitespace is removed and whitespace-only titles are rejected.
- [ ] Tests specify that IDs increase and are not reused after deletion.

**Likely files:**

- `todo.h`
- `tests/todo_list_tests.cpp`

**Verification:**

- [ ] The test source compiles against the declared interface once implementation is added.
- [ ] Each required domain behavior has at least one assert-based test.

**Scope:** Small — 2 files.

### Task 2: Implement and pass the domain behavior

Implement `TodoList` in `todo.cpp`. Trim title input before validation and storage. Generate increasing IDs, expose a read-only ordered task list, mark only an existing unfinished task as done, and delete only an existing task.

**Dependencies:** Task 1

**Acceptance criteria:**

- [ ] Valid titles are stored without leading or trailing whitespace.
- [ ] Empty or whitespace-only titles fail without creating a task.
- [ ] Marking a valid unfinished task as done succeeds; unknown or already-done IDs report failure without corrupting state.
- [ ] Deleting a valid task succeeds; deleting an unknown ID reports failure.
- [ ] Task IDs remain unique and are not reused.

**Likely files:**

- `todo.cpp`
- `todo.h`
- `tests/todo_list_tests.cpp`

**Verification:**

- [ ] Compile and run the assert test binary with `-std=c++17 -Wall -Wextra -pedantic`.
- [ ] All assertions pass with exit status `0`.

**Scope:** Small — 3 files.

### Checkpoint: Domain Foundation

- [ ] The domain tests pass.
- [ ] No console code owns task state or implements task mutation rules.
- [ ] The source uses only the C++ standard library.

### Task 3: Implement the testable console menu

Create `ConsoleApp` using injected `std::istream` and `std::ostream`. Render this fixed numbered menu: `1 = Add task`, `2 = List tasks`, `3 = Mark task done`, `4 = Delete task`, and `5 = Quit`. Read all entries as lines, parse IDs safely, and return to the menu after invalid input.

**Dependencies:** Task 2

**Acceptance criteria:**

- [ ] The menu explicitly maps `1` to Add task, `2` to List tasks, `3` to Mark task done, `4` to Delete task, and `5` to Quit.
- [ ] Listing displays each task’s ID, title, and an unambiguous pending or done indicator.
- [ ] Invalid menu choices, malformed IDs, blank titles, unknown IDs, and already-done IDs show feedback and keep the application usable.
- [ ] Input EOF ends the loop gracefully rather than causing an infinite loop or crash.

**Likely files:**

- `console_app.h`
- `console_app.cpp`

**Verification:**

- [ ] Compile the console code with the domain implementation.
- [ ] Exercise one normal session and one invalid-input session using redirected standard input.

**Scope:** Small — 2 files.

### Task 4: Wire the entry point and preserve the interactive runner

Replace the placeholder `main.cpp` with startup code that creates the to-do list and console application, then runs the menu loop. Update `test_runner.sh` only as needed to compile all application source files with C++17 and launch the app interactively. Add the literal `app` entry to `.gitignore` because `test_runner.sh` writes that root-level binary and the existing `*.app` pattern does not ignore it.

**Dependencies:** Task 3

**Acceptance criteria:**

- [ ] Running `test_runner.sh` builds the application and starts the interactive menu.
- [ ] The runner does not compile test sources into the application binary.
- [ ] The root-level `app` binary produced by the runner is ignored by Git.
- [ ] The application starts with an empty list and exits cleanly when quit is selected.

**Likely files:**

- `main.cpp`
- `test_runner.sh`
- `.gitignore`

**Verification:**

- [ ] Run `./test_runner.sh`.
- [ ] Add a task, list it, mark it done, delete it, and quit manually.

**Scope:** Small — 3 files.

### Task 5: Add a separate automated test runner and console integration test

Create `tests/run_tests.sh`. It should create the ignored `build/` directory with `mkdir -p build`, build the assert-based domain test binary separately from the application, run it, then build or invoke the application and pipe a deterministic console session into it. Use `set -e` so a failed build, assertion, or output check stops the script. Assert stable user-visible output fragments rather than a full terminal transcript.

**Dependencies:** Tasks 2 and 4

**Acceptance criteria:**

- [ ] The script verifies add → list → mark done → list → delete → list → quit.
- [ ] The script covers at least one invalid-input recovery path.
- [ ] The script creates `build/` before writing either compiled binary.
- [ ] The script exits non-zero when unit assertions fail or expected console output is missing.
- [ ] Test artifacts are written to an ignored build location, not committed to the repository.

**Likely files:**

- `tests/run_tests.sh`
- `tests/todo_list_tests.cpp`

**Verification:**

- [ ] Run `./tests/run_tests.sh` from the repository root.
- [ ] Confirm it completes non-interactively with exit status `0`.

**Scope:** Small — 2 files.

### Checkpoint: End-to-End MVP

- [ ] `test_runner.sh` launches the app successfully.
- [ ] `tests/run_tests.sh` passes without interactive input.
- [ ] The scripted flow confirms task state changes are visible to users.
- [ ] Invalid input does not terminate the app unexpectedly.

### Task 6: Document application usage and MVP boundaries

Update the README with build/run instructions, menu capabilities, automated test command, and the explicit session-only storage limitation. Optionally clarify the test-folder convention in `tests/README.md`.

**Dependencies:** Task 5

**Acceptance criteria:**

- [ ] Documentation provides a working app command and a working automated-test command.
- [ ] Documentation states that tasks are not retained after exit.
- [ ] Documentation does not promise unimplemented features.

**Likely files:**

- `README.md`
- `tests/README.md`

**Verification:**

- [ ] Follow the documented commands verbatim in the project container or equivalent C++ environment.
- [ ] Confirm documentation matches actual menu behavior.

**Scope:** Extra small — 1–2 files.

### Checkpoint: Ready for Review

- [ ] Build succeeds with warnings enabled.
- [ ] All assert-based and console integration tests pass.
- [ ] README commands work as documented.
- [ ] All MVP acceptance criteria are met.

# Testing Strategy

### Unit Tests

Use a plain C++ executable built from `tests/todo_list_tests.cpp` and `todo.cpp`, with `assert` checks for:

- adding a valid task;
- trimming surrounding whitespace;
- rejecting empty and whitespace-only titles;
- increasing IDs across multiple additions;
- marking a known pending task done;
- rejecting an unknown or already-done task ID;
- deleting an existing task;
- rejecting deletion of an unknown task; and
- preserving unaffected tasks after completion or deletion.

### Console Integration Test

Use `tests/run_tests.sh` to pipe a deterministic command sequence into the application. Verify that output includes:

- the added task title;
- its pending state;
- its done state after completion;
- deletion confirmation or an empty-list message after removal;
- invalid-input feedback; and
- a normal quit message.

### Edge Cases

- Empty menu selection.
- Non-numeric or out-of-range menu selection.
- Non-numeric, blank, or unknown task ID.
- Leading/trailing title whitespace.
- Whitespace-only title.
- Repeating “mark done” for an already done task.
- Listing an empty list.
- EOF from redirected input.

# Acceptance Criteria

- [ ] The application is a working C++17 console program with no third-party dependencies.
- [ ] Users can add a non-empty task, list tasks, mark a pending task done, delete a task, and quit.
- [ ] Titles are trimmed before storage; blank titles are rejected.
- [ ] Tasks have unique increasing numeric IDs that are not reused.
- [ ] Completed tasks remain visible until deleted.
- [ ] Invalid input produces understandable feedback and does not crash or terminate the menu loop.
- [ ] Task data is session-only and disappears when the program exits.
- [ ] `test_runner.sh` remains the interactive app build/run path.
- [ ] A separate script under `tests/` runs assert-based tests and the scripted console flow successfully.

# Validation Commands

Run the interactive application:

```sh
./test_runner.sh
```

Run all automated tests:

```sh
./tests/run_tests.sh
```

The test script should internally use equivalent explicit builds:

```sh
mkdir -p build

g++ -std=c++17 -Wall -Wextra -pedantic -I. tests/todo_list_tests.cpp todo.cpp -o build/todo_list_tests
./build/todo_list_tests

g++ -std=c++17 -Wall -Wextra -pedantic main.cpp todo.cpp console_app.cpp -o build/todo_app
printf '1\n  Buy milk  \n2\n3\n1\n2\n4\n1\n2\ninvalid\n5\n' | ./build/todo_app
```

# Notes

- “Mark done” is intentionally one-way in this MVP; no toggle or reopen action is included.
- The planned source layout remains flat to match the repository’s current small-project conventions.
- `test_runner.sh` stays focused on building and running the interactive app. Automated tests belong in the separate `tests/run_tests.sh` workflow.
- Persistence, editing, task ordering, due dates, and richer task metadata are suitable follow-on features once the MVP is accepted.

#!/bin/bash

set -euo pipefail

cd "$(dirname "$0")/.."
mkdir -p build

g++ -std=c++17 -Wall -Wextra -pedantic -I. tests/todo_list_tests.cpp todo.cpp -o build/todo_list_tests
./build/todo_list_tests

g++ -std=c++17 -Wall -Wextra -pedantic main.cpp todo.cpp console_app.cpp -o build/todo_app

output=$(printf '1\n   \n1\n  Buy milk  \n2\n3\nnot-a-number\n3\n1\n2\n4\n99\n4\n1\n2\ninvalid\n5\n' | ./build/todo_app)

assert_contains() {
    if ! grep -Fq "$1" <<<"$output"; then
        echo "Expected output to contain: $1" >&2
        exit 1
    fi
}

assert_contains "Task title cannot be empty."
assert_contains "Added task #1."
assert_contains "[pending] 1: Buy milk"
assert_contains "Task ID must be a positive number."
assert_contains "Marked task #1 done."
assert_contains "[done] 1: Buy milk"
assert_contains "Task was not found."
assert_contains "Deleted task #1."
assert_contains "No tasks."
assert_contains "Invalid menu selection."
assert_contains "Goodbye."

echo "All tests passed."

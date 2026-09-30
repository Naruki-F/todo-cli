#include "console_app.h"

#include <algorithm>
#include <cctype>
#include <limits>
#include <stdexcept>

ConsoleApp::ConsoleApp(TodoList& todoList, std::istream& input, std::ostream& output)
    : todoList_(todoList), input_(input), output_(output) {}

void ConsoleApp::run() {
    output_ << "To-Do List\n";

    while (true) {
        showMenu();

        std::string choice;
        if (!readLine("Choose an option: ", choice)) {
            output_ << "\nInput ended. Goodbye.\n";
            return;
        }

        if (choice == "1") {
            if (!addTask()) {
                return;
            }
        } else if (choice == "2") {
            listTasks();
        } else if (choice == "3") {
            if (!markTaskDone()) {
                return;
            }
        } else if (choice == "4") {
            if (!deleteTask()) {
                return;
            }
        } else if (choice == "5") {
            output_ << "Goodbye.\n";
            return;
        } else {
            output_ << "Invalid menu selection.\n";
        }
    }
}

void ConsoleApp::showMenu() const {
    output_ << "\n1. Add task\n"
            << "2. List tasks\n"
            << "3. Mark task done\n"
            << "4. Delete task\n"
            << "5. Quit\n";
}

bool ConsoleApp::addTask() {
    std::string title;
    if (!readLine("Task title: ", title)) {
        output_ << "\nInput ended. Goodbye.\n";
        return false;
    }

    const auto task = todoList_.add(title);
    if (!task) {
        output_ << "Task title cannot be empty.\n";
        return true;
    }

    output_ << "Added task #" << task->id << ".\n";
    return true;
}

bool ConsoleApp::listTasks() {
    const std::vector<Task>& tasks = todoList_.tasks();
    if (tasks.empty()) {
        output_ << "No tasks.\n";
        return true;
    }

    for (const Task& task : tasks) {
        output_ << (task.done ? "[done] " : "[pending] ") << task.id << ": " << task.title << '\n';
    }

    return true;
}

bool ConsoleApp::markTaskDone() {
    std::string value;
    if (!readLine("Task ID to mark done: ", value)) {
        output_ << "\nInput ended. Goodbye.\n";
        return false;
    }

    std::size_t id = 0;
    if (!parseTaskId(value, id)) {
        output_ << "Task ID must be a positive number.\n";
        return true;
    }

    if (!todoList_.markDone(id)) {
        output_ << "Task was not found or is already done.\n";
        return true;
    }

    output_ << "Marked task #" << id << " done.\n";
    return true;
}

bool ConsoleApp::deleteTask() {
    std::string value;
    if (!readLine("Task ID to delete: ", value)) {
        output_ << "\nInput ended. Goodbye.\n";
        return false;
    }

    std::size_t id = 0;
    if (!parseTaskId(value, id)) {
        output_ << "Task ID must be a positive number.\n";
        return true;
    }

    if (!todoList_.remove(id)) {
        output_ << "Task was not found.\n";
        return true;
    }

    output_ << "Deleted task #" << id << ".\n";
    return true;
}

bool ConsoleApp::readLine(const std::string& prompt, std::string& value) {
    output_ << prompt;
    return static_cast<bool>(std::getline(input_, value));
}

bool ConsoleApp::parseTaskId(const std::string& value, std::size_t& id) {
    if (value.empty() || !std::all_of(value.begin(), value.end(), [](unsigned char character) {
            return std::isdigit(character);
        })) {
        return false;
    }

    try {
        const unsigned long long parsedId = std::stoull(value);
        if (parsedId == 0 || parsedId > std::numeric_limits<std::size_t>::max()) {
            return false;
        }

        id = static_cast<std::size_t>(parsedId);
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }
}

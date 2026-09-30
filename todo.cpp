#include "todo.h"

#include <algorithm>
#include <cctype>

namespace {

std::string trim(const std::string& value) {
    const auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char character) {
        return std::isspace(character);
    });
    const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char character) {
        return std::isspace(character);
    }).base();

    if (first >= last) {
        return "";
    }

    return std::string(first, last);
}

}  // namespace

std::optional<Task> TodoList::add(const std::string& title) {
    const std::string trimmedTitle = trim(title);
    if (trimmedTitle.empty()) {
        return std::nullopt;
    }

    const Task task{nextId_++, trimmedTitle, false};
    tasks_.push_back(task);
    return task;
}

const std::vector<Task>& TodoList::tasks() const {
    return tasks_;
}

bool TodoList::markDone(std::size_t id) {
    for (Task& task : tasks_) {
        if (task.id == id && !task.done) {
            task.done = true;
            return true;
        }
    }

    return false;
}

bool TodoList::remove(std::size_t id) {
    const auto task = std::find_if(tasks_.begin(), tasks_.end(), [id](const Task& currentTask) {
        return currentTask.id == id;
    });

    if (task == tasks_.end()) {
        return false;
    }

    tasks_.erase(task);
    return true;
}

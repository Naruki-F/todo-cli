#ifndef TODO_H
#define TODO_H

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

struct Task {
    std::size_t id;
    std::string title;
    bool done;
};

class TodoList {
public:
    std::optional<Task> add(const std::string& title);
    const std::vector<Task>& tasks() const;
    bool markDone(std::size_t id);
    bool remove(std::size_t id);

private:
    std::vector<Task> tasks_;
    std::size_t nextId_ = 1;
};

#endif

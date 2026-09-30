#ifndef CONSOLE_APP_H
#define CONSOLE_APP_H

#include "todo.h"

#include <cstddef>
#include <istream>
#include <ostream>
#include <string>

class ConsoleApp {
public:
    ConsoleApp(TodoList& todoList, std::istream& input, std::ostream& output);

    void run();

private:
    void showMenu() const;
    bool addTask();
    bool listTasks();
    bool markTaskDone();
    bool deleteTask();
    bool readLine(const std::string& prompt, std::string& value);
    static bool parseTaskId(const std::string& value, std::size_t& id);

    TodoList& todoList_;
    std::istream& input_;
    std::ostream& output_;
};

#endif

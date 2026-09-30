#include "console_app.h"
#include "todo.h"

#include <iostream>

int main() {
    TodoList todoList;
    ConsoleApp app(todoList, std::cin, std::cout);
    app.run();
    return 0;
}

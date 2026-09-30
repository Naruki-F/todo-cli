#include "todo.h"

#include <cassert>

int main() {
    TodoList todoList;

    assert(todoList.tasks().empty());
    assert(!todoList.add(""));
    assert(!todoList.add("   \t  "));

    const auto firstTask = todoList.add("  Buy milk  ");
    assert(firstTask);
    assert(firstTask->id == 1);
    assert(firstTask->title == "Buy milk");
    assert(!firstTask->done);

    const auto secondTask = todoList.add("Call Mom");
    assert(secondTask);
    assert(secondTask->id == 2);
    assert(todoList.tasks().size() == 2);

    assert(todoList.markDone(firstTask->id));
    assert(todoList.tasks().front().done);
    assert(!todoList.markDone(firstTask->id));
    assert(!todoList.markDone(99));

    assert(todoList.remove(firstTask->id));
    assert(todoList.tasks().size() == 1);
    assert(todoList.tasks().front().id == secondTask->id);
    assert(!todoList.remove(firstTask->id));

    const auto thirdTask = todoList.add("Write tests");
    assert(thirdTask);
    assert(thirdTask->id == 3);
}

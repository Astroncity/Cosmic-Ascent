#pragma once
#include <list>
#include <stdbool.h>
#include <string>

#define TASK_PARAMS void *taskData, Task *task

/*typedef struct Task Task;
typedef struct Task {
    void* taskData; // if run needs external data
    bool setForDeletion;
    void (*run)(void* dataData, Task*);
    const char* name;
} Task;

typedef struct TaskNode TaskNode;
typedef struct TaskNode {
    Task* task;
    TaskNode* next;
} TaskNode;

Task* createTask(const char* name, void* taskData,
                 void (*run)(void*, Task*));
void runAllTasks();
void printRunningTasks();*/

class Task {
  public:
    void* taskData;
    bool setForDeletion;
    void (*run)(TASK_PARAMS);
    std::string name;
    static std::list<Task*> tasks;

    Task(std::string name, void* taskData, void (*run)(TASK_PARAMS));

    static void runAll();
    static void log();
};

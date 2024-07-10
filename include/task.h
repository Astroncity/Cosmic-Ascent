#pragma once
#include "defs.h"
#include <stdbool.h>

#define TASK_PARAMS void *taskData, Task *task

typedef struct Task Task;
typedef struct Task {
    void* taskData; // if run needs external data
    bool setForDeletion;
    void (*run)(void*, Task*);
} Task;

typedef struct TaskNode TaskNode;
typedef struct TaskNode {
    Task* task;
    TaskNode* next;
} TaskNode;

Task* createTask(void* taskData, void (*run)(void*, Task*));
void runAllTasks();

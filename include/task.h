#pragma once
#include <stdbool.h>

#define TASK_PARAMS void *taskData, Task *task

typedef struct Task Task;
typedef struct Task {
    void* taskData; // if run needs external data
    bool setForDeletion;
    void (*run)(void* /*dataData*/, Task*);
    char* name;
} Task;

typedef struct TaskNode TaskNode;
typedef struct TaskNode {
    Task* task;
    TaskNode* next;
} TaskNode;

Task* createTask(char* name, void* taskData, void (*run)(void*, Task*));
void runAllTasks();
void printRunningTasks();

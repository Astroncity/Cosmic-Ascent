#include "task.h"
#include <stdio.h>
#include <stdlib.h>

TaskNode* taskHead = NULL;

static void insertTask(Task* task) {
    TaskNode* tk = (TaskNode*)malloc(sizeof(TaskNode));
    tk->task = task;
    tk->next = NULL;

    TaskNode* curr = taskHead;

    if (taskHead == NULL) {
        taskHead = tk;
        return;
    }

    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = tk;
}

Task* createTask(const char* name, void* taskData,
                 void (*run)(void*, Task*)) {
    Task* task = (Task*)malloc(sizeof(Task));
    task->taskData = taskData;
    task->run = run;
    task->name = name;
    task->setForDeletion = false;

    insertTask(task);

    return task;
}

void runAllTasks() {
    TaskNode* curr = taskHead;
    TaskNode* prev = NULL;

    while (curr != NULL) {
        curr->task->run(curr->task->taskData, curr->task);

        if (curr->task->setForDeletion) {
            if (prev == NULL) {
                taskHead = curr->next;
            } else {
                prev->next = curr->next;
            }

            TaskNode* temp = curr;
            curr = curr->next;
            free(temp->task);
            free(temp);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
}

void printRunningTasks() {
    TaskNode* curr = taskHead;
    printf("[---COROUTINES---]\n");
    while (curr != NULL) {
        printf("%s -> ", curr->task->name);
        curr = curr->next;
    }
    printf("\n");
}

#include "task.h"
#include <stdlib.h>

TaskNode* taskHead = NULL;

static void insertTask(Task* task) {
    TaskNode* new = (TaskNode*)malloc(sizeof(TaskNode));
    new->task = task;
    new->next = NULL;

    TaskNode* curr = taskHead;

    if (taskHead == NULL) {
        taskHead = new;
        return;
    }

    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = new;
}

Task* createTask(void* taskData, void (*run)(void*, Task*)) {
    Task* task = (Task*)malloc(sizeof(Task));
    task->taskData = taskData;
    task->run = run;
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

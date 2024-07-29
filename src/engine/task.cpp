#include "task.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

std::list<Task*> Task::tasks;

Task::Task(std::string name, void* taskData, void (*run)(TASK_PARAMS)) {
    this->taskData = taskData;
    this->run = run;
    this->name = name;
    this->setForDeletion = false;
    tasks.push_back(this);
}

void Task::runAll() {
    for (std::list<Task*>::iterator it = tasks.begin(); it != tasks.end();
         ++it) {
        Task* task = *it;
        if (!task->setForDeletion) {
            task->run(task->taskData, task);
        } else {
            it = tasks.erase(it);
        }
    }
}

void Task::log() {
    printf("[---COROUTINES---]\n");
    for (auto task : tasks) {
        std::cout << task->name + " -> ";
    }
    printf("\n");
}

// #include <Timer.h>
#include "Queue.h"
#include <cstdio>

class Scheduler {
    public:
        const static unsigned int MAX_NUM_TASKS = 20;
    public:

        // bool addSequentialTask(bool (*)(float), float);
        // bool addParallelTask();
        // void loop();
        // bool pause();
        // bool resume();
bool addSequentialTask(bool (*task)(float), float arg=-1.0) {
    if (numTasks < MAX_NUM_TASKS) {
        taskQueue.enqueue(task);
        taskArgsQueue.enqueue(arg);
        priorityQueue.enqueue(curPriority);
        ++numTasks; ++curPriority;
        return true;
    }
    return false;
}

// Scheduler::addParallelTask(bool (*task)())

void loop() {
    if (!paused) {
        if (numTasks > 0) {
            if ((*(taskQueue.head->data))(taskArgsQueue.head->data)) {
                taskQueue.dequeue();
                taskArgsQueue.dequeue();
                priorityQueue.dequeue();
                --numTasks;
                printf("Done 1 task\n");
            }
        }
    }
}

bool pause() {
    if (!paused) {
        printf("pause");
        paused = true;
        return true;
    }
    return false;   
}

bool resume() {
    if (paused) {
        printf("resume");
        paused = false;    
        return true;
    }
    return false;   
}

    private:
        unsigned int numTasks = 0;
        // bool (*taskList[MAX_NUM_TASKS]) (float);
        Queue<bool (*)(float)> taskQueue;
        // float taskArgs[MAX_NUM_TASKS];
        Queue<float> taskArgsQueue;
        int curPriority = 0;
        Queue<int> priorityQueue;
        // int priorityList[MAX_NUM_TASKS];

        bool paused = true;
};
// #include <Timer.h>
#include "Queue.h"
#include <cstdio>

class Scheduler
{
public:
    enum class TaskType {SEQUENTIAL, PARALLEL};
    // const static unsigned int MAX_NUM_TASKS = 20;

public:
    // bool addSequentialTask(bool (*)(float), float);
    // bool addParallelTask();
    // void loop();
    // bool pause();
    // bool resume();

    bool addTask(bool (*task)(float), float arg=-1.0, TaskType tt=TaskType::SEQUENTIAL)
    {
        taskQueue.enqueue(task);
        taskArgsQueue.enqueue(arg);
        priorityQueue.enqueue(curPriority);
        ++numTasks;
        if (tt == TaskType::SEQUENTIAL) {
            ++curPriority;
        }
        return true;
    }

    // currently only supports no arg
    bool addPauseTask(bool (*ptask)(void)) {
        pauseTask = ptask;
        return true;
    }

    void loop()
    {
        if (!paused)
        {
            QueueNode<bool (*)(float)> *task = taskQueue.head;
            QueueNode<float> *taskArg = taskArgsQueue.head;
            QueueNode<int> *priority = priorityQueue.head;
            int prevPriority = priority->data;
            while (priority->data == prevPriority) {
                if ((*(task->data))(taskArg->data))
                {
                    taskQueue.dequeue();
                    taskArgsQueue.dequeue();
                    priorityQueue.dequeue();
                    --numTasks;
                    printf("Done 1 task\n");
                }
                if (task->nextPtr != NULL) {
                    task = task->nextPtr;
                    taskArg = taskArg->nextPtr;
                    priority = priority->nextPtr;
                } else break;
            }
        }
    }

    bool pause()
    {
        if (!paused)
        {
            printf("sched::pause");
            (*pauseTask)();
            paused = true;
            return true;
        }
        return false;
    }

    bool resume()
    {
        if (paused)
        {
            printf("sched::resume");
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
    bool (*pauseTask)(void);

    bool paused = true;
};
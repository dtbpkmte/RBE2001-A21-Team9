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
        taskStatusQueue.enqueue(false);
        printf("New task %p added with priority=%d\n", task, curPriority);
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
            int n = numTasks;
            if (n) {
                QueueNode<bool (*)(float)> *task = taskQueue.head;
                QueueNode<float> *taskArg = taskArgsQueue.head;
                QueueNode<int> *priority = priorityQueue.head;
                QueueNode<bool> *taskStatus = taskStatusQueue.head;
                int prevPriority = priority->data;
                while (priority->data == prevPriority) {
                    if (taskStatus->data)
                    {
                        if (task == taskQueue.head){
                            taskQueue.dequeue();
                            taskArgsQueue.dequeue();
                            priorityQueue.dequeue();
                            taskStatusQueue.dequeue();
                            --numTasks;
                            printf("Dequeued task %p\n", task->data);   
                            if (numTasks == 0) curPriority = 0;           
                        }
                    } else {
                        taskStatus->data = (*(task->data))(taskArg->data);
                        if (taskStatus->data)
                            printf("Task %p completed\n", task->data);
                    }
                    if (--n > 0) {
                        task = task->nextPtr;
                        taskArg = taskArg->nextPtr;
                        priority = priority->nextPtr;
                        taskStatus = taskStatus->nextPtr;
                    } else break;
                }
            }
        }
    }

    bool pause()
    {
        if (!paused)
        {
            printf("sched::pause\n");
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
            printf("sched::resume\n");
            paused = false;
            return true;
        }
        return false;
    }

    bool terminateCurrentTask() {
        if (numTasks) {
            taskQueue.dequeue();
            taskArgsQueue.dequeue();
            priorityQueue.dequeue();
            taskStatusQueue.dequeue();
            --numTasks;
            return true;
        }
        return false;
    }

private:
    unsigned int numTasks = 0;
    // bool (*taskList[MAX_NUM_TASKS]) (float);
    Queue<bool (*)(float)> taskQueue;
    Queue<bool> taskStatusQueue;
    // float taskArgs[MAX_NUM_TASKS];
    Queue<float> taskArgsQueue;
    int curPriority = 0;
    Queue<int> priorityQueue;
    // int priorityList[MAX_NUM_TASKS];
    bool (*pauseTask)(void);

    bool paused = true;
};
// #include "Scheduler.h"

// // Scheduler::Scheduler();
// // Scheduler::~Scheduler();

// bool Scheduler::addSequentialTask(bool (*task)(float), float arg=-1.0) {
//     if (numTasks < MAX_NUM_TASKS) {
//         taskQueue.enqueue(task);
//         taskArgsQueue.enqueue(arg);
//         priorityQueue.enqueue(curPriority);
//         ++numTasks; ++curPriority;
//         return true;
//     }
//     return false;
// }

// // Scheduler::addParallelTask(bool (*task)())

// void Scheduler::loop() {
//     if (!paused) {
//         if (numTasks > 0) {
//             if ((*(taskQueue.head->data))(taskArgsQueue.head->data)) {
//                 taskQueue.dequeue();
//                 taskArgsQueue.dequeue();
//                 priorityQueue.dequeue();
//                 --numTasks;
//             }
//         }
//     }
// }

// bool Scheduler::pause() {
//     if (!paused) {
//         paused = true;
//         return true;
//     }
//     return false;   
// }

// bool Scheduler::resume() {
//     if (paused) {
//         paused = false;    
//         return true;
//     }
//     return false;   
// }
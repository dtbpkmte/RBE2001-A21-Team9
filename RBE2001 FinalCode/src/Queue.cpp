// #include "Queue.h"

// template<class T> void Queue<T>::enqueue(T val) {
//     QueueNode<T> *newNode = malloc(sizeof(QueueNode<T>));
//     if (newNode) {
//         newNode->data = val;
//         newNode->nextPtr = NULL;
//         if (head == NULL) {
//             head = newNode;
//         } else {
//             tail->nextPtr = newNode;
//         }
//         tail = newNode;
//         ++queueLength;
//     }
// }


// template<class T> T Queue<T>::dequeue() {
//     QueueNode<T> *tmp = head;
//     T ret = tmp->data;
//     head = tmp->nextPtr;
//     free(tmp);
//     --queueLength;
//     return ret;
// }

// template<class T> unsigned int Queue<T>::len() {
//     return queueLength;
// }


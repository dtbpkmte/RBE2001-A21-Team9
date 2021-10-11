// #include <iostream>
#include <cstdlib>
#include <Utility.h>

template<class T>
class QueueNode {
    public:
        QueueNode();
        ~QueueNode();

        T data;
        QueueNode *nextPtr;
};

template<class T>
class Queue {

    public:
        QueueNode<T> *head = NULL;
        QueueNode<T> *tail = NULL;

    public:
        // void enqueue(T data);
        // T dequeue();
        // unsigned int len();
        void enqueue(T val) {
            QueueNode<T> *newNode = (QueueNode<T>*) malloc(sizeof(QueueNode<T>));
            if (newNode) {
                newNode->data = val;
                newNode->nextPtr = NULL;
                if (head == NULL) {
                    head = newNode;
                } else {
                    tail->nextPtr = newNode;
                }
                tail = newNode;
                ++queueLength;
            }
        }


        T dequeue() {
            QueueNode<T> *tmp = head;
            T ret = tmp->data;
            head = tmp->nextPtr;
            free(tmp);
            --queueLength;
            return ret;
        }

        unsigned int len() {
            return queueLength;
        }

    private:
        unsigned int queueLength = 0;
};
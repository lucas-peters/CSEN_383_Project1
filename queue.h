#ifndef QUEUE_H
#define QUEUE_H
#include "process.h"
#include <stdio.h>
#include <stdlib.h>

struct QueueNode {
    struct Process process;
    struct QueueNode* next;
};


int insert(struct QueueNode* q, struct Process p);
struct QueueNode* pop(struct QueueNode* q);
void free_queue(struct QueueNode* q);

#endif

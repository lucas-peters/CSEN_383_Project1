#include "queue.h"

struct QueueNode* pop(struct QueueNode* q) {
    if (!q)
        return NULL;
    
    struct QueueNode* old_head = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    
    while (1) {
        if (!old_head->next)
            return old_head;
        old_head = old_head->next; 
    }    
}

int insert(struct QueueNode* q, struct Process process) {
    if(!q)
        return 0;

    struct QueueNode* temp = q;
    struct QueueNode* new_node = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    new_node->process = process;
    new_node->next = NULL;
    
    while(temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = new_node;

    return 1;
}

void freeQueue(struct QueueNode* q) {
    struct QueueNode* curr = q;
    struct QueueNode* next;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
}

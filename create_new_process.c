#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "create_new_process.h"
#include "utility_structures.h"

int compare(void *val1, void *val2) {
    proc *p1 = val1;
    proc *p2 = val2;

    if (p1->arrival_time < p2->arrival_time) return -1;
    if (p1->arrival_time == p2->arrival_time) return 0;
    return 1;
}

proc *create_new_process(char pid, float arrival_time, float burst_time, unsigned char priority) {
    proc *new_process = malloc(sizeof(proc));

    new_process->proc_id = pid;
    new_process->arrival_time = arrival_time;
    new_process->burst_time = burst_time;
    new_process->priority = priority;

    return new_process;
}

llist *generate_processes(int n) {
    llist *process_list = create_newlist();
    if (!process_list) {
        fprintf(stderr, "Unable to create Linked List\n");
    }

    char proc_id = 'A';
    unsigned char priority;
    float arrival_time, burst_time;

    srand(5);

    for (int i = 0; i < n; i++) {
        arrival_time = rand() % 100;
        burst_time = (rand() % 100 + 1) / 10.0f;
        priority = (rand() % 4) + 1;

        proc *new_proc = create_new_process(proc_id++, arrival_time, burst_time, priority);
        insert_newnode(process_list, new_proc);
    }

    sort(process_list, compare);

    node *current_node = process_list->head;
    proc_id = 'A';
    while (current_node) {
        proc *current_proc = current_node->data;
        current_proc->proc_id = proc_id++;

        if (proc_id > 'Z') proc_id = 'a';

        current_node = current_node->next;
    }

    return process_list;
}


#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"
#include "utility_structures.h"

process_times *create_process_stats(proc *process) {
    process_times *pt = malloc(sizeof(process_times));

    if (!pt) {
        perror("Failed to allocate memory for process_times");
        exit(EXIT_FAILURE);
    }

    pt->p = process;
    pt->wait_time = 0;
    pt->turn_around_time = 0;
    pt->response_time = 0;
    pt->burst_time = 0;
    pt->start_time = -1;
    pt->end_time = -1;

    return pt;
}

all_stats first_come_first_serve(llist *procs) {
    int t = 0;
    queue *proc_q = new_queue();

    if (!proc_q) {
        perror("Failed to allocate memory for queue");
        exit(EXIT_FAILURE);
    }

    node *proc_ptr = procs->head;

    if (!proc_ptr) {
        fprintf(stderr, "No Process to schedule\n");
    }

    process_times *scheduled_proc = NULL;
    llist *l = create_newlist();

    if (!l) {
        perror("Failed to allocate memory for llist");
        exit(EXIT_FAILURE);
    }

    printf("\nFIRST COME FIRST SERVE:\n");

    while (t < 100 || scheduled_proc) {
        if (proc_ptr) {
            proc *new_process = proc_ptr->data;

            if (new_process->arrival_time <= t) {
                add_queue(proc_q, create_process_stats(new_process));
                proc_ptr = proc_ptr->next;
            }
        }

        if (!scheduled_proc && proc_q->size > 0) {
            scheduled_proc = del_queue(proc_q);
        }

        if (scheduled_proc) {
            proc *process = scheduled_proc->p;
            printf("%c", process->proc_id);

            if (scheduled_proc->start_time == -1) {
                scheduled_proc->start_time = t;
            }

            scheduled_proc->burst_time++;

            if (scheduled_proc->burst_time >= process->burst_time) {
                scheduled_proc->end_time = t;
                insert_newnode(l, scheduled_proc);
                scheduled_proc = NULL;
            }
        } else {
            printf("_");
        }

        t++;
    }

    printf("\n");
    return print_all_stats(l);
}


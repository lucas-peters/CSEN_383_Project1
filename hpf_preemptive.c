#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"
#include "utility_structures.h"

process_times *create_process_stats(proc *process);

int priority_comparison(void *d1, void *d2) {
    process_times *process1 = d1;
    process_times *process2 = d2;
    proc *proc1 = process1->p;
    proc *proc2 = process2->p;

    if (proc1->priority < proc2->priority) {
        return -1;
    }

    if (proc1->priority == proc2->priority && proc1->arrival_time < proc2->arrival_time) {
        return -1;
    }

    return 1;
}

all_stats hpf_p(llist *procs) {
    int time = 0;

    llist *lists[4] = {
        create_newlist(),
        create_newlist(),
        create_newlist(),
        create_newlist()
    };

    queue *queues[4] = {
        new_queue(),
        new_queue(),
        new_queue(),
        new_queue()
    };

    node *proc_pointer = procs->head;

    printf("\nHIGHEST PRIORITY FIRST PREEMPTIVE:\n");
    process_times *scheduled_proc = NULL;

    while (time < 100 || scheduled_proc) {
        if (scheduled_proc) {
            add_queue(queues[scheduled_proc->p->priority - 1], scheduled_proc);
        }

        if (proc_pointer) {
            proc *new_proc = proc_pointer->data;

            while (proc_pointer && new_proc->arrival_time <= time) {
                add_queue(queues[new_proc->priority - 1], create_process_stats(new_proc));
                
                for (int i = 0; i < 4; i++) {
                    sort(queues[i], priority_comparison);
                }

                proc_pointer = proc_pointer->next;
                if (proc_pointer) {
                    new_proc = proc_pointer->data;
                }
            }
        }

        if (!scheduled_proc) {
            for (int i = 0; i < 4 && !scheduled_proc; i++) {
                if (queues[i]->size > 0) {
                    scheduled_proc = del_queue(queues[i]);
                }
            }

            if (time >= 100 && scheduled_proc && scheduled_proc->start_time == -1) {
                scheduled_proc = NULL;
                continue;
            }
        }

        if (scheduled_proc) {
            proc *process = scheduled_proc->p;

            printf("%c", process->proc_id);

            if (scheduled_proc->start_time == -1) {
                scheduled_proc->start_time = time;
            }

            scheduled_proc->burst_time++;

            if (scheduled_proc->burst_time >= process->burst_time) {
                scheduled_proc->end_time = time;
                insert_newnode(lists[process->priority - 1], scheduled_proc);
                scheduled_proc = NULL;
            }
        }

        time++;
    }

    all_stats avgs[4];
    char *labels[] = {
        "\nFor Priority Queue 1",
        "\nFor Priority Queue 2",
        "\nFor Priority Queue 3",
        "\nFor Priority Queue 4"
    };

    all_stats avg = {0};

    for (int i = 0; i < 4; i++) {
        printf("%s", labels[i]);
        avgs[i] = print_all_stats(lists[i]);
        avg.avg_response_time += avgs[i].avg_response_time;
        avg.avg_wait_time += avgs[i].avg_wait_time;
        avg.avg_turn_around += avgs[i].avg_turn_around;
        avg.avg_throughput += avgs[i].avg_throughput;
    }

    avg.avg_response_time /= 4;
    avg.avg_wait_time /= 4;
    avg.avg_turn_around /= 4;

    printf("\nThe average times of High Priority First Preemptive for all queues:\n");
    printf("Average Turn Around Time(TAT) :%.1f\n", avg.avg_turn_around);
    printf("Average Wait Time(WT) : %.1f\n", avg.avg_wait_time);
    printf("Average Response Time(RT) : %.1f\n", avg.avg_response_time);

    return avg;
}


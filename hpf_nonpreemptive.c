#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"
#include "utility_structures.h"

process_times *create_process_stats(proc *process);
int compare_priority(void *data1, void *data2);
void printqueue(queue *q);

all_stats hpf_np(llist *procs) {
    int time = 0;
    node *proc_pointer = procs->head;

    queue *queues[4] = {
        new_queue(),
        new_queue(),
        new_queue(),
        new_queue()
    };

    llist *lists[4] = {
        create_newlist(),
        create_newlist(),
        create_newlist(),
        create_newlist()
    };


    printf("\nHIGHEST PRIORITY FIRST NON PREEMPTIVE:\n");
    process_times *scheduled_process = NULL;

    while (time < 100 || scheduled_process) {
        if (proc_pointer) {
            proc *new_proc = proc_pointer->data;

            while (proc_pointer && new_proc->arrival_time <= time) {
                if (new_proc->priority >= 1 && new_proc->priority <= 4) {
                    add_queue(queues[new_proc->priority - 1], create_process_stats(new_proc));
                }
                proc_pointer = proc_pointer->next;
                if (proc_pointer) {
                    new_proc = proc_pointer->data;
                }
            }
        }

        if (!scheduled_process) {
            for (int i = 0; i < 4 && !scheduled_process; i++) {
                if (queues[i]->size > 0) {
                    scheduled_process = del_queue(queues[i]);
                }
            }

            if (time >= 100 && scheduled_process->start_time == -1) {
                scheduled_process = NULL;
                continue;
            }
        }

        if (scheduled_process) {
            proc *process = scheduled_process->p;

            printf("%c", process->proc_id);

            if (scheduled_process->start_time == -1) {
                scheduled_process->start_time = time;
            }

            scheduled_process->burst_time++;

            if (scheduled_process->burst_time >= process->burst_time) {
                scheduled_process->end_time = time;
                insert_newnode(lists[process->priority - 1], scheduled_process);
                scheduled_process = NULL;
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

    printf("The Calculated statistics of High Priority First Non Preemptive for all priority queues is:\n");
    printf("Average Turn Around Time(TAT) :%.1f\n", avg.avg_turn_around);
    printf("Average Wait Time(WT) : %.1f\n", avg.avg_wait_time);
    printf("Average Response Time(RT) : %.1f\n", avg.avg_response_time);

    return avg;
}


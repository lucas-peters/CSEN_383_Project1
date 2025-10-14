#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"
#include "utility_structures.h"

process_times *create_process_stats(proc *process);
int compare_remaining_time(void *d1, void *d2);

all_stats srt(llist *procs) {
    int time = 0;

    queue *proc_queue = new_queue();
    llist *completed_proc = create_newlist();

    node *current_node = procs->head;
    process_times *current_process = NULL;

    printf("\n\n SHORTEST REMAINING TIME:\n");

    while (time < 100 || current_process) {
        if (current_process) {
            add_queue(proc_queue, current_process);
            current_process = NULL;
        }

        if (current_node) {
            proc *new_proc_entry = current_node->data;
            
            while (current_node && new_proc_entry->arrival_time <= time) {
                add_queue(proc_queue, create_process_stats(new_proc_entry));
                
                current_node = current_node->next;
                if (current_node) {
                    new_proc_entry = current_node->data;
                }
            }

            sort(proc_queue, compare_remaining_time);
        }

        if (!current_process && proc_queue->size) {
            current_process = del_queue(proc_queue);

            while (time >= 100 && current_process->start_time == -1) {
                current_process = del_queue(proc_queue);
            }
        }

        if (current_process) {
            proc *process_detail = current_process->p;
            printf("%c", process_detail->proc_id);

            if (current_process->start_time == -1) {
                current_process->start_time = time;
            }

            current_process->burst_time++;

            if (current_process->burst_time >= process_detail->burst_time) {
                current_process->end_time = time;
                insert_newnode(completed_proc, current_process);
                current_process = NULL;
            }
        }

        time++;
    }

    return print_all_stats(completed_proc);
}

int compare_remaining_time(void *d1, void *d2) {
    process_times *p_state_1 = d1;
    process_times *p_state_2 = d2;
    
    int remaining_time_1 = p_state_1->p->burst_time - p_state_1->burst_time;
    int remaining_time_2 = p_state_2->p->burst_time - p_state_2->burst_time;

    return remaining_time_1 < remaining_time_2 ? -1 : 1;
}


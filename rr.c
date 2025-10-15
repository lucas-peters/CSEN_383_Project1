#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"
#include "utility_structures.h"

process_times *create_process_stats(proc *process);

all_stats round_robin(llist *processes, int time_slice) {
    int time = 0;
    int cur_run = 0;
    node *current_node = NULL;

    queue *proc_queue = new_queue();
    llist *l = create_newlist();

    if (!processes->head) {
        fprintf(stderr, "NO PROCESSES\n");
    }

    node *proc_ptr = processes->head;
    printf("\nROUND ROBIN:\n");

    while (time < 100 || proc_queue->size > 0) {

        if (proc_ptr && time < 100) {
            proc *new_proc = proc_ptr->data;

            while (proc_ptr && new_proc->arrival_time <= time) {
                add_queue(proc_queue, create_process_stats(new_proc));
                proc_ptr = proc_ptr->next;

                if (proc_ptr) {
                    new_proc = proc_ptr->data;
                }
            }
        }

        if (!current_node) {
            current_node = proc_queue->head;
            cur_run = 0;
        } else if (cur_run == time_slice) {
            cur_run = 0;
            current_node = current_node->next ? current_node->next : proc_queue->head;
        }

        if (current_node) {
            process_times *scheduled_proc = current_node->data;
            proc *process = scheduled_proc->p;

            if (time >= 100 && scheduled_proc->start_time == -1) {
                node *next_node = current_node->next;
                del_node(proc_queue, current_node->data);
                current_node = next_node;
                cur_run = 0;
                continue;
            }

            printf("%c", process->proc_id);
            cur_run++;

            if (scheduled_proc->start_time == -1) {
                scheduled_proc->start_time = time;
            }

            scheduled_proc->burst_time++;

            if (scheduled_proc->burst_time >= process->burst_time) {
                scheduled_proc->end_time = time;
                insert_newnode(l, scheduled_proc);
                node *next_node = current_node->next;
                del_node(proc_queue, current_node->data);
                current_node = next_node;
                cur_run = 0;
            }
        }

        time++;
    }

    return print_all_stats(l);
}


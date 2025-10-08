#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_ARRIVAL_TIME 99
#define MAX_RUN_TIME 10

struct Process {
    float arrival_time;
    float run_time;
    int priority;
};

struct Process* generate_process();
struct Process** create_proc_list(int num_procs);
int partition(struct Process** proc_list, int lo, int hi);
int rand_partition(struct Process** proc_list, int lo, int hi);
void quick_sort(struct Process** proc_list, int lo, int hi);
void sort_proc_list(struct Process** proc_list, int num_procs); // sort by arrival time
void free_procs(struct Process** proc_list, int num_procs);

#endif

#include "process.h"

struct Process* generate_process() {
    struct Process* process = (struct Process*)malloc(sizeof(struct Process));
    process->arrival_time = rand() % 100;
    process->run_time = (rand() % 10) + 1;
    process->priority = (rand() % 4) + 1;
    return process;
}

struct Process** create_proc_list(int num_procs) {
    if (num_procs <= 0)
        return NULL;

    struct Process** proc_list = (struct Process**)malloc(num_procs * sizeof(struct Process*));
    for (int i = 0; i < num_procs; i++) {
        proc_list[i] = generate_process();
    }
    return proc_list;
}

// Theses functions implement a randomized quicksort algorithm to sort the processes by time arrival order
int partition(struct Process** proc_list, int lo, int hi) {
    int pivot = proc_list[lo]->arrival_time;
    int i = lo, j = hi;

    while(1) {
        while(proc_list[i]->arrival_time < pivot) {
            i++;
        }

        while (proc_list[j]->arrival_time > pivot) {
            j--;
        }

        if (i >= j)
            return j;

        struct Process* temp = proc_list[i];
        proc_list[i] =  proc_list[j];
        proc_list[j] = temp;
    }
}
int rand_partition(struct Process** proc_list, int lo, int hi) {
    int random = lo + rand() % (hi - lo + 1);
    
    struct Process* temp = proc_list[random];
    proc_list[random] = proc_list[lo];
    proc_list[lo] = temp;
    
    return partition(proc_list, lo, hi);
}
// randomized quick_sort O(nlogn) worst case
void quick_sort(struct Process** proc_list, int lo, int hi) {
    if (lo < hi) {
        int partition_index = rand_partition(proc_list, lo, hi);
        quick_sort(proc_list, lo, partition_index);
        quick_sort(proc_list, partition_index + 1, hi);
    }
}

void sort_proc_list(struct Process** proc_list, int num_procs) {
    srand(time(0));
    quick_sort(proc_list, 0, num_procs - 1);
}

void free_procs(struct Process** proc_list, int num_procs) {
    for (int i = 0; i < num_procs; i++) {
        free(proc_list[i]);
    }
    free(proc_list);
}

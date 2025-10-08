#include "queue.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
    int num_procs = 0;
    if (argc < 2)
        num_procs = 10;

    srand(time(0));
    int tat[num_procs]; // turn-around-time
    int rpt[num_procs]; // response-time
    int wt[num_procs]; // wait_time
    int curr_time = 0;

    struct Process** proc_list = create_proc_list(num_procs);
    struct Process* proc;
    sort_proc_list(proc_list, num_procs);

    // fcfs logic
    for (int i = 0; i < num_procs; i++) {
        proc = proc_list[i];
        
        // cpu is idle
        if (curr_time <= proc->arrival_time) {
            wt[i] = 0;
            rpt[i] = 0;
        } else { // cpu was busy
            wt[i] = curr_time - proc->arrival_time;
            rpt[i] = wt[i];
        }
        tat[i] = wt[i] + proc->run_time;
        curr_time += proc->run_time;
    }

    int total_wt = 0, total_rpt = 0, total_tat = 0;

    for (int i = 0; i < num_procs; i++) {
        total_wt += wt[i];
        total_rpt += rpt[i];
        total_tat += tat[i];
    }
    
    printf("Average Tunaround time: %f secpmds\n", (float)total_tat / (float)num_procs);
    printf("Average Response Time: %f seconds\n", (float)total_rpt / (float)num_procs);
    printf("Average Wait Time: %f seconds\n", (float)total_wt / (float)num_procs);
    free_procs(proc_list, num_procs);
    return 0;
}

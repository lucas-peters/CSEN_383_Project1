#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_PROCS  200

static int pick_shortest_job(float *arrival, float *run, int *done, int n, float now)
{
    int idx = -1;
    float best = 1e9;
    for (int i = 0; i < n; i++) {
        if (!done[i] && arrival[i] <= now && run[i] < best) {
            best = run[i];
            idx  = i;
        }
    }
    return idx;
}

int main(int argc, char **argv)
{
    int num_procs = (argc >= 2) ? atoi(argv[1]) : 50; //default 50
    struct Process **plist = create_proc_list(num_procs);
    sort_proc_list(plist, num_procs);

    float arrival[num_procs], burst[num_procs];
    for (int i = 0; i < num_procs; i++) {
        arrival[i] = plist[i]->arrival_time;
        burst[i]   = plist[i]->run_time;
    }

    // statistics arrays
    float rem[num_procs];     memcpy(rem, burst, sizeof(rem));
    float resp[num_procs];    for (int i = 0; i < num_procs; i++) resp[i] = -1;
    float tat[num_procs], wt[num_procs];
    int   done[num_procs];    memset(done, 0, sizeof(done));

    float now = 0; int finished = 0;
    while (finished < num_procs) {
        int idx = pick_shortest_job(arrival, rem, done, num_procs, now);
        if (idx == -1) { // jump to next
            float next_arr = 1e9;
            for (int i = 0; i < num_procs; i++)
                if (!done[i] && arrival[i] > now && arrival[i] < next_arr)
                    next_arr = arrival[i];
            now = next_arr;
            continue;
        }
        if (resp[idx] < 0) resp[idx] = now - arrival[idx];

        now += rem[idx]; // run to completion
        tat[idx]  = now - arrival[idx];
        wt[idx]   = tat[idx] - burst[idx];
        rem[idx]  = 0;
        done[idx] = 1;
        finished++;
    }

    // averages
    float sum_tat = 0, sum_wt = 0, sum_resp = 0;
    for (int i = 0; i < num_procs; i++) {
        sum_tat  += tat[i];
        sum_wt   += wt[i];
        sum_resp += resp[i];
    }
    printf("\nSJF (%d procs)\n", num_procs);
    printf("Average Turnaround Time: %.3f quanta\n",  sum_tat  / num_procs);
    printf("Average Waiting Time:    %.3f quanta\n",  sum_wt   / num_procs);
    printf("Average Response Time:   %.3f quanta\n",  sum_resp / num_procs);
    printf("Throughput: %.3f procs/quantum\n",
           (float)num_procs / now);

    free_procs(plist, num_procs);
    return 0;
}

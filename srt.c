#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static int pick_shortest_remaining(float *arrival, float *rem, int n, int t_now)
{
    int best = -1;
    float left = 1e9;
    for (int i = 0; i < n; i++) {
        if (arrival[i] <= t_now && rem[i] > 0 && rem[i] < left) {
            left = rem[i];
            best = i;
        }
    }
    return best;
}

int main(int argc, char **argv)
{
    int num_procs = (argc >= 2) ? atoi(argv[1]) : 50;
    struct Process **plist = create_proc_list(num_procs);
    sort_proc_list(plist, num_procs);

    float arrival[num_procs], burst[num_procs], rem[num_procs];
    for (int i = 0; i < num_procs; i++) {
        arrival[i] = plist[i]->arrival_time;
        burst[i]   = plist[i]->run_time;
        rem[i]     = burst[i];
    }

    float resp[num_procs];  for (int i = 0; i < num_procs; i++) resp[i] = -1;
    float tat[num_procs], wt[num_procs];
    int   done_cnt = 0, time = 0;

    // run until every process is finished
    while (done_cnt < num_procs) {
        int idx = pick_shortest_remaining(arrival, rem, num_procs, time);
        if (idx == -1) { time++; continue; } // nothing ready, idle

        if (resp[idx] < 0) resp[idx] = time - arrival[idx]; // first touch

        rem[idx] -= 1;
        time++;

        if (rem[idx] <= 0) { // completed this quantum
            tat[idx] = time - arrival[idx];
            wt[idx]  = tat[idx] - burst[idx];
            done_cnt++;
        }
    }

    float sum_tat = 0, sum_wt = 0, sum_resp = 0;
    for (int i = 0; i < num_procs; i++) {
        sum_tat  += tat[i];
        sum_wt   += wt[i];
        sum_resp += resp[i];
    }
    printf("\nSRT (%d procs)\n", num_procs);
    printf("Average Turnaround Time: %.3f quanta\n",  sum_tat  / num_procs);
    printf("Average Waiting Time:    %.3f quanta\n",  sum_wt   / num_procs);
    printf("Average Response Time:   %.3f quanta\n",  sum_resp / num_procs);
    printf("Throughput: %.3f procs/quantum\n",
           (float)num_procs / time);

    free_procs(plist, num_procs);
    return 0;
}

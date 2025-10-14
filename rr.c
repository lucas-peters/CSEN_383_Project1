#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

static void q_init(int *q, int *head, int *tail, int cap) {
    (void)cap; // cap unused here but kept for clarity/consistency
    *head = 0; *tail = 0;
}

static int q_empty(int head, int tail) {
    return head == tail;
}

static void q_push(int *q, int *tail, int cap, int v) {
    q[*tail] = v;
    *tail = (*tail + 1) % cap;
}

static int q_pop(int *q, int *head, int cap) {
    int v = q[*head];
    *head = (*head + 1) % cap;
    return v;
}

int main(int argc, char **argv)
{
    int num_procs = (argc >= 2) ? atoi(argv[1]) : 50;
    if (num_procs <= 0) {
        fprintf(stderr, "num_procs must be > 0\n");
        return 1;
    }

    struct Process **plist = create_proc_list(num_procs);
    sort_proc_list(plist, num_procs);

    float arrival[num_procs], burst[num_procs], rem[num_procs];
    for (int i = 0; i < num_procs; i++) {
        arrival[i] = plist[i]->arrival_time;
        burst[i]   = plist[i]->run_time;
        rem[i]     = burst[i];
    }

    float resp[num_procs];    
	for (int i = 0; i < num_procs; i++) 
		resp[i] = -1.0f;
    float tat[num_procs];     
	memset(tat, 0, sizeof(tat));
    float wt[num_procs];      
	memset(wt,  0, sizeof(wt));
    int   finished = 0;
    int   started[num_procs]; memset(started, 0, sizeof(started));

    int cap = (num_procs < 1 ? 1 : num_procs) * 2; 
    int *Q = (int*)malloc(sizeof(int) * cap);
    int head, tail; q_init(Q, &head, &tail, cap);

    int time = 0;
    int enq[num_procs]; memset(enq, 0, sizeof(enq));

    for (int i = 0; i < num_procs; i++) {
        if (arrival[i] <= 0.0f && rem[i] > 0 && !enq[i]) {
            q_push(Q, &tail, cap, i);
            enq[i] = 1;
        }
    }

    while (finished < num_procs) {
        if (q_empty(head, tail)) {
            float next_arr = 1e9f;
            for (int i = 0; i < num_procs; i++) {
                if (rem[i] > 0 && arrival[i] > (float)time && arrival[i] < next_arr) {
                    next_arr = arrival[i];
                }
            }
            if (next_arr < 1e8f) {
                time = (int)next_arr;
                for (int i = 0; i < num_procs; i++) {
                    if (arrival[i] <= (float)time && rem[i] > 0 && !enq[i]) {
                        q_push(Q, &tail, cap, i);
                        enq[i] = 1;
                    }
                }
            } else {
                break;
            }
            continue;
        }

        int idx = q_pop(Q, &head, cap);
        enq[idx] = 0;

        if (resp[idx] < 0.0f) {
            resp[idx] = (float)time - arrival[idx];
            if (!started[idx]) started[idx] = 1;
        }

        if (rem[idx] > 0.0f) rem[idx] -= 1.0f;
        time += 1;

        for (int i = 0; i < num_procs; i++) {
            if (rem[i] > 0 && !enq[i] && arrival[i] <= (float)time && !started[i]) {
                q_push(Q, &tail, cap, i);
                enq[i] = 1;
            }
        }

        if (rem[idx] > 0.0f) {
            if (!enq[idx]) { q_push(Q, &tail, cap, idx); enq[idx] = 1; }
        } else {
            tat[idx] = (float)time - arrival[idx];
            wt[idx]  = tat[idx] - burst[idx];
            finished++;
        }
    }


    float sum_tat = 0.0f, sum_wt = 0.0f, sum_resp = 0.0f;
    for (int i = 0; i < num_procs; i++) {
        sum_tat  += tat[i];
        sum_wt   += wt[i];
        sum_resp += resp[i];
    }
    printf("\nRR (q=1, %d procs)\n", num_procs);
    printf("Average Turnaround Time: %.3f quanta\n",  sum_tat  / num_procs);
    printf("Average Waiting Time:    %.3f quanta\n",  sum_wt   / num_procs);
    printf("Average Response Time:   %.3f quanta\n",  sum_resp / num_procs);
    printf("Throughput: %.3f procs/quantum\n",
           (float)num_procs / (float)time);

    free(Q);
    free_procs(plist, num_procs);
    return 0;
}

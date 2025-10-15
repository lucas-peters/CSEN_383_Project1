#ifndef _statistics_h_
#define _statistics_h_

#include "create_new_process.h"

typedef struct {
    proc *p;
    float wait_time;
    float turn_around_time;
    float response_time;
    float start_time;
    float end_time;
    float burst_time;
} process_times;

typedef struct {
    llist *process_queue;
    llist *time_chart;
    float avg_turn_around_time;
    float avg_waiting_time;
    float avg_response_time;
    float throughput;
    int total_quanta;
} stats;

typedef struct {
    float avg_response_time;
    float avg_wait_time;
    float avg_turn_around;
    float avg_throughput;
} all_stats;

// Function prototypes
all_stats first_come_first_serve(llist *p);
all_stats shortest_job_first(llist *p);
all_stats srt(llist *p);
all_stats round_robin(llist *p, int time_slice);
all_stats hpf_p(llist *p);
all_stats hpf_np(llist *p);
all_stats print_all_stats(llist *l);

#endif


#include <stdio.h>
#include "utility_structures.h"
#include "create_new_process.h"
#include "statistics.h"



void print_stats(const char *label, all_stats stats) {
    printf("%s\n", label);
    printf("Average Turn Around Time(TAT) :%.1f\n", stats.avg_turn_around);
    printf("Average Wait Time(WT) : %.1f\n", stats.avg_wait_time);
    printf("Average Response Time(RT) : %.1f\n", stats.avg_response_time);
    printf("Average Throughput :%.1f\n", stats.avg_throughput);
    printf("\n");
}

int main(int argc, char **argv)
{
	int ctr = 0;

	all_stats fcfs[6], sjf[6], srf[6], rr[6], hpfp[6], hpfnp[6], final[6];

	for (int i = 0; i < 6; i++)
	{
		final[i].avg_response_time = 0;
		final[i].avg_wait_time = 0;
		final[i].avg_turn_around = 0;
		final[i].avg_throughput = 0;
	}
	
	int num_proc = 0;
	printf("Enter number of processes: ");
	scanf("%d", &num_proc);

	while (ctr < 5)
	{
		printf("\t\t\t\tIteration #%d\n", ctr + 1);
		
		llist *processList = generate_processes(num_proc);
		node *ptr = processList->head;

		printf("process_name | arrival_time | burst_time | process_priority |\n");

		while (ptr != NULL)
		{
			proc *p = (proc *)ptr->data;
			printf("%10c | %12.1f | %8.1f | %8d |\n", p->proc_id, p->arrival_time, p->burst_time, p->priority);
			ptr = ptr->next;
		}

		printf("total_number_of_processes: %d\n", processList->size);

		fcfs[ctr] = first_come_first_serve(processList);
		rr[ctr] = round_robin(processList, 5);
		sjf[ctr] = shortest_job_first(processList);
		srf[ctr] = srt(processList);
		hpfp[ctr] = hpf_p(processList);
		hpfnp[ctr] = hpf_np(processList);
		ctr++;
	}

	// Addition of all the given stats
	for (int i = 0; i < 5; i++)
	{
		final[0].avg_response_time += fcfs[i].avg_response_time;
		final[0].avg_wait_time += fcfs[i].avg_wait_time;
		final[0].avg_turn_around += fcfs[i].avg_turn_around;
		final[0].avg_throughput += fcfs[i].avg_throughput;

		final[1].avg_response_time += sjf[i].avg_response_time;
		final[1].avg_wait_time += sjf[i].avg_wait_time;
		final[1].avg_turn_around += sjf[i].avg_turn_around;
		final[1].avg_throughput += sjf[i].avg_throughput;

		final[2].avg_response_time += srf[i].avg_response_time;
		final[2].avg_wait_time += srf[i].avg_wait_time;
		final[2].avg_turn_around += srf[i].avg_turn_around;	
		final[2].avg_throughput += srf[i].avg_throughput;

		final[3].avg_response_time += rr[i].avg_response_time;
		final[3].avg_wait_time += rr[i].avg_wait_time;
		final[3].avg_turn_around += rr[i].avg_turn_around;
		final[3].avg_throughput += rr[i].avg_throughput;

		final[4].avg_response_time += hpfp[i].avg_response_time;
		final[4].avg_wait_time += hpfp[i].avg_wait_time;
		final[4].avg_turn_around += hpfp[i].avg_turn_around;
		final[4].avg_throughput += hpfp[i].avg_throughput;

		final[5].avg_response_time += hpfnp[i].avg_response_time;
		final[5].avg_wait_time += hpfnp[i].avg_wait_time;		
		final[5].avg_turn_around += hpfnp[i].avg_turn_around;
		final[5].avg_throughput += hpfnp[i].avg_throughput;
	}

	for (int i = 0; i < 6; i++)
	{
		final[i].avg_response_time /= 5;
		final[i].avg_wait_time /= 5;
		final[i].avg_turn_around /= 5;
		final[i].avg_throughput /= 5;
	}

	
	
	printf("\nAll STATISTICS FOR 5 ITERATIONS\n\n");
        print_stats("FIRST COME FIRST SERVE (FCFS):", final[0]);
        print_stats("SHORTEST JOB FIRST (SJF):", final[1]);
        print_stats("SHORTEST REMAINING TIME (SRT):", final[2]);
        print_stats("ROUND ROBIN (RR):", final[3]);
        print_stats("HIGHEST PRIORITY FIRST (HPF) [NON-PREEMPTIVE]:", final[5]);
        print_stats("HIGHEST PRIORITY FIRST (HPF) [PREEMPTIVE]:", final[4]);
}

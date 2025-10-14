#include <stdio.h>
#include <stdlib.h>
#include "statistics.h"
#include "utility_structures.h"

process_times *create_process_stats(proc *process);

int compare_time(void *, void *);

all_stats shortest_job_first(llist *procs)
{
	int time = 0;

	queue *proc_queue = (queue *)new_queue();
	node *proc_ptr = procs->head;
	if (procs->head == NULL)
		fprintf(stderr, "NO PROC\n");

	process_times *scheduled_proc = NULL;

	llist *l = create_newlist();
	
	printf("\nSHORTEST JOB FIRST:\n");
	
	while (time < 100 || scheduled_proc != NULL)
	{
		if (proc_ptr != NULL)
		{
			proc *new_proc = (proc *)(proc_ptr->data);
			while (proc_ptr != NULL && new_proc->arrival_time <= time)
			{
				add_queue(proc_queue, create_process_stats(new_proc));
				sort(proc_queue, compare_time);
				proc_ptr = proc_ptr->next;

				if (proc_ptr != NULL)
					new_proc = (proc *)(proc_ptr->data);
			}
		}

		if (scheduled_proc == NULL && proc_queue->size > 0)
		{
			scheduled_proc = (process_times *)del_queue(proc_queue);
		}

		if (scheduled_proc != NULL)
		{
			proc *process = scheduled_proc->p;

			printf("%c", process->proc_id);

			if (scheduled_proc->start_time == -1)
			{
				scheduled_proc->start_time = time;
			}
			scheduled_proc->burst_time++;

			if (scheduled_proc->burst_time >= process->burst_time)
			{
				scheduled_proc->end_time = time;
				insert_newnode(l, scheduled_proc);
				scheduled_proc = NULL;
			}
		}

		time++;
	}
	printf("\n");

	return print_all_stats(l);
}

int compare_time(void *data1, void *data2)
{
	process_times *pt1 = (process_times *)data1;
	process_times *pt2 = (process_times *)data2;
	if (((proc *)pt1->p)->burst_time < ((proc *)pt2->p)->burst_time)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

#ifndef _process_h_
#define _process_h_

#include "utility_structures.h"

struct each_process {
	unsigned char proc_id;
	float arrival_time;
	float burst_time;
	unsigned char priority; 
};

typedef struct each_process proc;

proc * create_new_process(char proc_id, float arrival_time, float burst_time, unsigned char priority);

//proc * get_copy_of_process(proc * p);

llist * generate_processes(int n);

#endif
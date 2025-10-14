#include <stdio.h>
#include <stdlib.h>
#include "utility_structures.h"
#include "create_new_process.h"
#include "statistics.h"

llist *create_newlist() {
    llist *l = malloc(sizeof(llist));
    if (!l) return NULL;
    
    l->head = l->tail = NULL;
    l->size = 0;
    return l;
}

node *create_newnode(void *val) {
    node *n = malloc(sizeof(node));
    if (!n) return NULL;

    n->data = val;
    n->next = n->prev = NULL;
    return n;
}

void insert_newnode(llist *l, void *val) {
    node *n = create_newnode(val);
    if (!n) return;

    if (!l->head) {
        l->head = l->tail = n;
    } else {
        l->tail->next = n;
        n->prev = l->tail;
        l->tail = n;
    }
    l->size++;
}

void insert_between(llist *l, node *cur, void *val) {
    if (!l || !cur) return;

    node *n = create_newnode(val);
    if (!n) return;

    n->prev = cur;
    n->next = cur->next;

    if (cur->next) cur->next->prev = n;
    cur->next = n;
    if (l->tail == cur) l->tail = n;

    l->size++;
}

void del_node(llist *l, void *val) {
    if (!l) return;

    node *n = l->head;
    while (n && n->data != val) n = n->next;

    if (n) {
        if (n->prev) n->prev->next = n->next;
        if (n->next) n->next->prev = n->prev;
        if (n == l->head) l->head = n->next;
        if (n == l->tail) l->tail = n->prev;

        free(n);
        l->size--;
    }
}

void del_headnode(llist *l) {
    if (!l || !l->head) return;

    node *n = l->head;
    l->head = n->next;

    if (l->head) l->head->prev = NULL;
    if (l->tail == n) l->tail = NULL;

    free(n);
    l->size--;
}

queue *new_queue() {
    return create_newlist();
}

void add_queue(queue *q, void *val) {
    if (!q) return;

    node *n = create_newnode(val);
    if (!n) return;

    if (!q->head) {
        q->head = q->tail = n;
    } else {
        q->tail->next = n;
        n->prev = q->tail;
        q->tail = n;
    }
    q->size++;
}

void *del_queue(queue *q) {
    if (!q || !q->head) return NULL;

    node *n = q->head;
    void *val = n->data;

    q->head = n->next;
    if (q->head) q->head->prev = NULL;
    if (q->tail == n) q->tail = NULL;

    free(n);
    q->size--;

    return val;
}

void swap_nodes(node *a, node *b) {
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;
}

void sort(llist *l, int (*cmp)(void *, void *)) {
    for (node *i = l->head; i; i = i->next) {
        for (node *j = i->next; j; j = j->next) {
            if (cmp(i->data, j->data) > 0) swap_nodes(i, j);
        }
    }
}


all_stats print_all_stats(llist *l) 
{
    all_stats stats;
    
    
    
    float total_response_time = 0;
    float total_wait_time = 0;
    float total_turn_around = 0;
    int count = 0;

    printf("\nprocess_name | arrival_time | start_time | end_time | burst_time | response_time | wait_time | turn_around_time | priority |\n");
    
    printf("\n -------------------------------------------------------------------------------------------------------------------------------- \n");

    for (node *n = l->head; n; n = n->next) {
        process_times *p_times = n->data;
        proc *p = p_times->p;

        float arr_time = p->arrival_time;
        float burst = p->burst_time;
        float response_time = p_times->start_time - arr_time;
        float turn_around = p_times->end_time - arr_time + 1;
        float wait_time = turn_around - burst;

        total_response_time += response_time;
        total_wait_time += wait_time;
        total_turn_around += turn_around;
        count++;

        printf( "%13c|%14.1f|%12.1f|%10.1f|%10.1f|%15.1f|%11.1f|%17.1f|%10u|\n", p->proc_id, arr_time, p_times->start_time, p_times->end_time, burst, response_time, wait_time, turn_around, p->priority);
    }

    stats.avg_response_time = count ? total_response_time / count : 0;
    stats.avg_wait_time = count ? total_wait_time / count : 0;
    stats.avg_turn_around = count ? total_turn_around / count : 0;
    stats.avg_throughput = count;

    printf( "%13s|%14.1f|%12.1f|%10.1f|%10.1f|%15.1f|%11.1f|%17.1f|\n", "Average", 0.0, 0.0, 0.0, 0.0, stats.avg_response_time, stats.avg_wait_time, stats.avg_turn_around);
    

    return stats;
}


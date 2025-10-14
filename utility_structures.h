#ifndef _utility_h_
#define _utility_h_

struct Node
{
	struct Node *next;
	struct Node *prev;
	void *data;
};

typedef struct Node node;

struct LinkedList
{
	node *head;
	node *tail;
	int size;
};

typedef struct LinkedList llist;

node *create_newnode(void *data);
llist *create_newlist();
void insert_newnode(llist *l, void *data);
void del_node(llist *l, void *data);
void insert_between(llist *l, node *position, void *data);
void sort(llist *l, int (*cmp)(void *val1, void *val2));
void swap(node *x, node *y);

typedef struct LinkedList queue;

queue *new_queue();
void add_queue(queue *q, void *data);
void *del_queue(queue *q);


#endif

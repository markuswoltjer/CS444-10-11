#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int data;
	struct node * next;
} node_t;

void free_queue(node_t ** head);
void print_queue(node_t * head);

node_t * create_node(int val);
node_t * insert_node(node_t * head, int val);
node_t * remove_node(node_t * curr, int val);
int search_by_value(node_t * head, int val);

#endif

#ifndef GRAPHS_H
#define GRAPHS_H

#include "../lib/hash.h"
#include "structures.h"

// used in the parse_graph function
#define MAX_LINE_SIZE 1000

// init_GNode: initializes a GNode
// returns a pointer to it
struct GNode *init_GNode(char *name);

// connect: adds a string to the connections list of a GNode
void connect(struct GNode *node, char *str);

// parse_graph: parses a file containing a graph configuration (see 'test.txt' for an example), adds
// all nodes to a hashtable
// returns a node list of all nodes created or NULL
struct Node *parse_graph(FILE *file, struct Hashtable *hash);

// search: performs a breadth-first search on a hashtable containing a graph
// returns a node list representing the shortest path from 'start' to 'goal', or NULL if no path is found
struct Node *search(struct Hashtable *hash, char *start, char *goal);

// reset_graph: reset all nodes to a pre-search state, should be called before
// a second search is performed
void reset_graph(struct Node *graph);

// dequeue: takes an item from the queue 'q'
struct GNode *dequeue(struct Queue *q);

// enqueue: enqueue an item in the queue 'q'
void enqueue(struct Queue *q, struct GNode *node);

#endif

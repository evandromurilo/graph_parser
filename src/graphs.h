#ifndef GRAPHS_H
#define GRAPHS_H

#include "../lib/hash.h"

// used in the parse_graph function
#define MAX_LINE_SIZE 1000

// a graph node
struct GNode {
	char *name;
	struct LinkedNode *connections;
	// variables from here on are used only during searches
	char *parent_name;
	bool checked;
	int layer;
};

// init_GNode: initializes a GNode
// returns a pointer to it
struct GNode *init_GNode(char *name);

// connect: adds a string to the connections list of a GNode
void connect(struct GNode *node, char *str);

// parse_graph: parses a file containing a graph configuration (see 'test.txt' for an example), adds
// all nodes to a hashtable
// returns a node list of all nodes created or NULL
struct Node *parse_graph(FILE *file, struct Hashtable *hash);

// reconstruct_path: takes a GNode at the end of a search and returns the path that led to it
struct Node *reconstruct_path(struct Hashtable *hash, struct GNode *end);

// search: performs a deapth-first search on a hashtable containing a graph
// returns a node list representing the shortest path from 'start' to 'goal', or NULL if no path is found
struct Node *df_search(struct Hashtable *hash, char *start, char *goal);

// search: performs a breadth-first search on a hashtable containing a graph
// returns a node list representing the shortest path from 'start' to 'goal', or NULL if no path is found
struct Node *bf_search(struct Hashtable *hash, char *start, char *goal);

// reset_graph: reset all nodes to a pre-search state, should be called before
// a second search is performed
void reset_graph(struct Node *graph);

#endif

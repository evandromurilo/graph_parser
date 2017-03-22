#ifndef STRUCTURES_H
#define STRUCTURES_H

// a graph node
struct GNode {
	char *name;
	char *parent_name;
	bool checked;
	int layer;
	struct StringNode *connections;
};

struct StringNode {
	char *str;
	struct StringNode *next;
};

struct StringList {
	int size;
	struct StringNode *first;
	struct StringNode *last;
};

struct Hashtable {
	int size;
	int stored;
	struct Node **values;
};

struct Node {
	struct GNode *value;
	char *key;
	struct Node *next;
};

struct QNode {
	struct GNode *value;
	struct QNode *next;
};

struct Queue {
	struct QNode *first;
	struct QNode *last;
};

#endif

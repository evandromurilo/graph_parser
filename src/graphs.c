#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../lib/m_basics.h"
#include "graphs.h"

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Use: ./graphs 'graphfile.txt'.\n");
		return 1;
	}

	FILE *file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("File '%s' not found.\n", argv[1]);
		return 1;
	}

	struct Hashtable hash;
	hash_init(&hash);

	struct Node *graph = parse_graph(file, &hash);

	printf("Type 'help' for help.\n");
	printf("Enter queries ('start' 'goal'):\n## ");
	int buffer_size = 0;
	char *buffer[2];
	char last[100];
	struct Node *(*search)(struct Hashtable*, char*, char*) = bf_search;
	while (scanf("%s", last) > 0) {
		if (buffer_size == 0 && strcmp(last, "help") == 0) {
			printf(">> df: depth-first search\n");
			printf(">> bf: breadth-first search\n");
			printf(">> example: df a l\n");
			printf("## ");
		}
		else if (buffer_size == 0 && strcmp(last, "df") == 0) {
			search = df_search;
		}
		else if (buffer_size == 0 && strcmp(last, "bf") == 0) {
			search = bf_search;
		}
		else buffer[buffer_size++] = concat("", last);

		if (buffer_size == 2) {
			buffer_size = 0;
			struct Node *path = search(&hash, buffer[0], buffer[1]);
			reset_graph(graph);

			printf(">> ");
			while (path != NULL) {
				printf("%s ", path->key);
				path = path->next;
			}
			printf("\n## ");
		}
	}

	return 0;
}

struct Node *parse_graph(FILE *file, struct Hashtable *hash) {
	char line[MAX_LINE_SIZE];
	struct Node *graph = NULL;

	// create GNodes and add them to the graph linked list
	while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
		struct StringList words;
		words.first = words.last = NULL;
		words.size = 0;

		split_linked(&words, line, " \t\n");
		if (words.first == NULL) continue;

		struct GNode *newG = init_GNode(words.first->str);
		for (struct StringNode *curr = words.first->next; curr != NULL; curr = curr->next) {
			connect(newG, curr->str);
		}

		struct Node *newN = malloc(sizeof (struct Node));
		newN->key = newG->name;
		newN->value = newG;
		newN->next = graph;
		graph = newN;
	}

	// add all created nodes to hashtable
	for (struct Node *curr = graph; curr != NULL; curr = curr->next) {
		hash_add(hash, curr->key, curr->value);
	}

	fclose(file);

	return graph;
}

void reset_graph(struct Node *graph) {
	while (graph != NULL) {
		graph->value->checked = false;
		graph->value->layer = 0;
		graph->value->parent_name = NULL;

		graph = graph->next;
	}
}

void enqueue(struct Queue *q, struct GNode *node) {
	struct QNode *new = malloc(sizeof(struct QNode));
	new->next = NULL;
	new->value = node;

	if (q->first == NULL) q->first = q->last = new;
	else {
		q->last->next = new;
		q->last = new;
	}
}

struct GNode *dequeue(struct Queue *q) {
	if (q->first == NULL) return NULL;
	else {
		struct GNode *temp = q->first->value;
		q->first = q->first->next;
		return temp;
	}
}

struct Node *df_search(struct Hashtable *hash, char *start, char *goal) {
	struct GNode *nstart = hash_getv(hash, start);
	nstart->checked = true;

	if (strcmp(start, goal) == 0) {
		// recreate and return the path to goal (nstar)
		struct Node *first = malloc(sizeof (struct Node));
		first->value = nstart;
		first->next = NULL;
		first->key = nstart->name;

		struct Node *new;
		while (first->value->parent_name != NULL) {
			new = hash_getn(hash, first->value->parent_name);
			new->next = first;
			first = new;
		}

		return first; 
	}

	// recursively search all children of nstart
	for (struct StringNode *snd = nstart->connections; snd != NULL; snd = snd->next) {
		struct GNode *temp = hash_getv(hash, snd->str);
		temp->parent_name = nstart->name;

		struct Node *path = df_search(hash, snd->str, goal);
		if (path != NULL) return path;
	}

	return NULL;
}

struct Node *bf_search(struct Hashtable *hash, char *start, char *goal) {
	struct GNode *nstart = hash_getv(hash, start);
	nstart->checked = true;
	nstart->parent_name = NULL;

	if (strcmp(start, goal) == 0) {
		struct Node *node = malloc(sizeof (struct Node));
		node->value = nstart;
		node->next = NULL;
		node->key = nstart->name;
		return node;
	}

	struct Queue q;
	q.first = q.last = NULL;
	enqueue(&q, nstart);

	struct GNode *curr;
	while ((curr = dequeue(&q)) != NULL) {
		if (strcmp(curr->name, goal) == 0) {
			// recreate and return the path to goal (curr)
			struct Node *first = malloc(sizeof (struct Node));
			first->value = curr;
			first->next = NULL;
			first->key = curr->name;

			struct Node *new;
			while (curr->parent_name != NULL) {
				new = hash_getn(hash, curr->parent_name);
				new->next = first;
				first = new;
				curr = new->value;
			}

		       	return first; 
		}

		// check current node and add every connection to the queue
		for (struct StringNode *snd = curr->connections; snd != NULL; snd = snd->next) {
			struct GNode *temp = hash_getv(hash, snd->str);

			if (!(temp->checked)) {
				enqueue(&q, temp);
				temp->layer = curr->layer + 1;
				temp->parent_name = curr->name;
				temp->checked = true;
			}
		}
	}

	return NULL;
}

void connect(struct GNode *node, char *str) {
	struct StringNode *new = malloc(sizeof (struct StringNode));
	new->str = concat("", str);
	new->next = node->connections;
	node->connections = new;
}

struct GNode *init_GNode(char *name) {
	struct GNode *new = malloc(sizeof (struct GNode));
	new->name = concat("", name);
	new->checked = false;
	new->connections = NULL;
	new->layer = 0;
	new->parent_name = NULL;

	return new;
}

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../lib/m_list.h"
#include "../lib/m_basics.h"
#include "graphs.h"

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Use:: ./graphs 'graphfile.txt'.\n");
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
	putchar('\n');

	return 0;
}

struct Node *parse_graph(FILE *file, struct Hashtable *hash) {
	char line[MAX_LINE_SIZE];
	struct Node *graph = NULL;

	// create GNodes and add them to the graph linked list
	while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
		struct LinkedList *words = init_list();

		split_linked(words, line, " \t\n");
		if (words->length == 0) continue;

		struct GNode *newG = init_GNode(pop_first(words));
		for (char *curr = pop_first(words); curr != NULL; curr = pop_first(words)) {
			connect(newG, curr);
			free(curr);
		}
		free(words);

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
		struct GNode *graphv = graph->value;
		graphv->checked = false;
		graphv->layer = 0;
		graphv->parent_name = NULL;

		graph = graph->next;
	}
}

struct Node *reconstruct_path(struct Hashtable *hash, struct GNode *end) {
	struct Node *first = malloc(sizeof (struct Node));
	first->value = end;
	first->next = NULL;
	first->key = end->name;

	struct Node *new;
	while (((struct GNode *)(first->value))->parent_name != NULL) {
		new = hash_getn(hash, ((struct GNode *)(first->value))->parent_name);
		new->next = first;
		first = new;
	}

	return first; 
}

struct Node *df_search(struct Hashtable *hash, char *start, char *goal) {
	struct GNode *nstart = hash_getv(hash, start);
	if (nstart == NULL) return NULL;
	nstart->checked = true;

	if (strcmp(start, goal) == 0) {
		return reconstruct_path(hash, nstart);
	}

	// recursively search all children of nstart
	for (struct LinkedNode *snd = nstart->connections; snd != NULL; snd = snd->next) {
		struct GNode *temp = hash_getv(hash, snd->value);
		if (temp->checked) continue;

		temp->parent_name = nstart->name;

		struct Node *path = df_search(hash, snd->value, goal);
		if (path != NULL) return path;
	}

	return NULL;
}

struct Node *bf_search(struct Hashtable *hash, char *start, char *goal) {
	struct GNode *nstart = hash_getv(hash, start);
	if (nstart == NULL) return NULL;

	nstart->checked = true;

	struct LinkedList *q = init_list();
	append_to_list(q, nstart);

	struct GNode *curr;
	while ((curr = pop_first(q)) != NULL) {
		if (strcmp(curr->name, goal) == 0) {
			return reconstruct_path(hash, curr);
		}

		// add every unchecked connection to the queue
		for (struct LinkedNode *snd = curr->connections; snd != NULL; snd = snd->next) {
			struct GNode *temp = hash_getv(hash, snd->value);

			if (!(temp->checked)) {
				append_to_list(q, temp);
				temp->layer = curr->layer + 1;
				temp->parent_name = curr->name;
				temp->checked = true;
			}
		}
	}

	return NULL;
}

void connect(struct GNode *node, char *str) {
	struct LinkedNode *new = malloc(sizeof (struct LinkedNode));
	new->value = concat("", str);
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

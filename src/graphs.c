#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../lib/m_list.h"
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

	struct Hashtable *hash = hash_init(hash);
	struct LinkedList *graph = parse_graph(file, hash);

	printf("Type 'help' for help.\n");
	printf("Enter queries ('start' 'goal'):\n## ");
	int buffer_size = 0;
	char *buffer[2];
	char last[100];
	struct LinkedList *(*search)(struct Hashtable*, char*, char*, struct LinkedList*) = bf_search;
	while (scanf("%s", last) > 0) {
		if (buffer_size == 0 && strcmp(last, "help") == 0) {
			printf(">> df: depth-first search\n");
			printf(">> bf: breadth-first search\n");
			printf(">> id: iterative deepening search\n");
			printf(">> example: df a l\n");
			printf("## ");
		}
		else if (buffer_size == 0 && strcmp(last, "df") == 0) {
			search = df_search;
		}
		else if (buffer_size == 0 && strcmp(last, "bf") == 0) {
			search = bf_search;
		}
		else if (buffer_size == 0 && strcmp(last, "id") == 0) {
			search = id_search;
		}
		else buffer[buffer_size++] = concat("", last);

		if (buffer_size == 2) {
			buffer_size = 0;
			struct LinkedList *path = search(hash, buffer[0], buffer[1], graph);
			reset_graph(graph);

			printf(">> ");
			if (path != NULL) {
				for (struct GNode *curr = pop_first(path); curr != NULL; curr = pop_first(path)) {
					printf("%s ", curr->name);
				}
			}
			printf("\n## ");
		}
	}
	putchar('\n');

	return 0;
}

struct LinkedList *parse_graph(FILE *file, struct Hashtable *hash) {
	char line[MAX_LINE_SIZE];
	struct LinkedList *graph = init_list();

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

		append_to_list(graph, newG);
	}

	// add all created nodes to hashtable
	for (struct LinkedNode *curr = graph->first; curr != NULL; curr = curr->next) {
		struct GNode *temp = curr->value;
		hash_add(hash, temp->name, temp);
	}

	fclose(file);

	return graph;
}

void reset_graph(struct LinkedList *graph) {
	for (struct LinkedNode *curr = graph->first; curr != NULL; curr = curr->next) {
		struct GNode *graphv = curr->value;
		graphv->checked = false;
		graphv->layer = 0;
		graphv->parent_name = NULL;
	}
}

struct LinkedList *reconstruct_path(struct Hashtable *hash, struct GNode *end) {
	struct LinkedList *path = init_list();
	prepend_to_list(path, end);

	while (((struct GNode *)(path->first->value))->parent_name != NULL) {
		prepend_to_list(path, hash_getv(hash, ((struct GNode *)(path->first->value))->parent_name));
	}

	return path; 
}

struct LinkedList *df_search(struct Hashtable *hash, char *start, char *goal, struct LinkedList *graph) {
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

		struct LinkedList *path = df_search(hash, snd->value, goal, graph);
		if (path != NULL) return path;
	}

	return NULL;
}

struct LinkedList *bf_search(struct Hashtable *hash, char *start, char *goal, struct LinkedList *graph) {
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

struct LinkedList *id_search(struct Hashtable *hash, char *start, char *goal, struct LinkedList *graph) {
	struct GNode *nstart = hash_getv(hash, start);
	if (nstart == NULL) return NULL;

	int max_layer = 2;

	while (true) {
		struct LinkedList *stack = init_list();
		prepend_to_list(stack, nstart);
		nstart->checked = true;

		struct GNode *curr;
		while (curr = pop_first(stack)) {
			if (strcmp(curr->name, goal) == 0) return reconstruct_path(hash, curr);
			else if (curr->layer+1 > max_layer) break;

			for (struct LinkedNode *nd = curr->connections; nd != NULL; nd = nd->next) {
				struct GNode *gnd = hash_getv(hash, nd->value);

				if (!(gnd->checked)) {
					gnd->checked = true;
					gnd->layer = curr->layer + 1;
					gnd->parent_name = curr->name;
					prepend_to_list(stack, gnd);
				}
			}
		}

		if (stack->length == 0) return NULL;
		reset_graph(graph);
		++max_layer;
		free(stack);
	}
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


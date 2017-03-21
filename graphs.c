#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lib/hash.h"
#include "lib/m_basics.h"

struct GNode *init_GNode(char *name);
void connect(struct GNode *node, char *str);
struct Node *search(struct Hashtable *hash, char *start, char *goal);
struct GNode *dequeue(struct Queue *q);
void enqueue(struct Queue *q, struct GNode *node);
struct Node *parse_graph(char *filename, struct Hashtable *hash);

int main(void) {
	struct Hashtable hash;
	hash_init(&hash);

	parse_graph("test2.txt", &hash);
	printf("End of parse\n");
	/* struct Node *path = search(&hash, "murilo", "josi"); */
	struct Node *path = search(&hash, "a", "h");
	printf("End of search\n");

	while (path != NULL) {
		printf("%s ", path->key);
		path = path->next;
	}
	putchar('\n');

	return 0;
}

struct Node *parse_graph(char *filename, struct Hashtable *hash) {
	FILE *file = fopen(filename, "r");
	char line[100];
	struct Node *graph = NULL;

	while (fgets(line, 100, file) != NULL) {
		struct StringList words;
		words.first = words.last = NULL;
		words.size = 0;

		split_linked(&words, 100, line, ' ');

		struct StringNode *curr = words.first;
		printf("creating GNode %s\n", curr->str);
		struct GNode *newG = init_GNode(curr->str);
		while ((curr = curr->next) != NULL && curr->str[0] != '\n') {
			connect(newG, curr->str);
		}

		struct Node *newN = malloc(sizeof (struct Node));
		newN->value = newG;
		newN->next = graph;
		newN->key = newG->name;
		graph = newN;
	}

	struct Node *curr = graph;
	while (curr != NULL) {
		printf("adding %s to hash\n", curr->key);
		hash_add(hash, curr->key, curr->value);
		curr = curr->next;
	}

	fclose(file);

	return graph;
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

	printf("enqueued %s\n", node->name);
}

struct GNode *dequeue(struct Queue *q) {
	if (q->first == NULL) {
		printf("dequeued NULL\n");
		return NULL;
	}
	else {
		struct GNode *temp = q->first->value;
		q->first = q->first->next;
		printf("dequeued %s\n", temp->name);
		return temp;
	}
}


struct Node *search(struct Hashtable *hash, char *start, char *goal) {
	printf("Search start\n");
	if (strcmp(start, goal) == 0) return 0;

	int n = 0;
	struct GNode *nstart = hash_getv(hash, start);
	struct GNode *curr;
	struct Queue q;

	printf("1\n");
	nstart->checked = true;
	nstart->parent_name = NULL;
	q.first = q.last = NULL;
	printf("2\n");

	enqueue(&q, nstart);

	while ((curr = dequeue(&q)) != NULL) {
		printf("looking at %s\n", curr->name);
		if (strcmp(curr->name, goal) == 0) {
			struct Node *first = malloc(sizeof (struct Node));
			first->value = curr;
			first->next = NULL;
			first->key = curr->name;

			struct Node *new;
			while (curr->parent_name != NULL) {
				printf("searching parent %s\n", curr->parent_name);
				new = hash_getn(hash, curr->parent_name);
				if (new == NULL) printf("failed to get parent %s\n", curr->parent_name);
				printf("got parent %s\n", new->key);
				new->next = first;
				first = new;
				curr = new->value;
			}


		       	return first; 
		}
		++n;

		for (struct StringNode *snd = curr->connections; snd != NULL; snd = snd->next) {
			printf("searching for %s\n", snd->str);
			struct GNode *temp = hash_getv(hash, snd->str);
			printf("got %s\n", temp->name);

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

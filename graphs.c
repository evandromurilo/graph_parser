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

int main(void) {
	struct Hashtable hash;
	hash_init(&hash);

	struct GNode *murilo = init_GNode("murilo");
	connect(murilo, "iana");
	connect(murilo, "beatriz");
	connect(murilo, "oxito");

	struct GNode *iana = init_GNode("iana");
	connect(iana, "murilo");
	connect(iana, "alan");
	connect(iana, "josi");

	struct GNode *alan = init_GNode("alan");
	connect(alan, "edgar");

	struct GNode *edgar = init_GNode("edgar");
	struct GNode *beatriz = init_GNode("beatriz");
	struct GNode *josi = init_GNode("josi");

	struct GNode *oxito = init_GNode("oxito");
	connect(oxito, "mariana");
	connect(oxito, "victor");

	struct GNode *mariana = init_GNode("mariana");

	struct GNode *victor = init_GNode("victor");
	connect(victor, "valdeir");
	connect(victor, "igor");
	connect(victor, "alan");

	struct GNode *valdeir = init_GNode("valdeir");
	struct GNode *igor = init_GNode("igor");

	hash_add(&hash, "murilo", murilo);
	hash_add(&hash, "iana", iana);
	hash_add(&hash, "beatriz", beatriz);
	hash_add(&hash, "josi", josi);
	hash_add(&hash, "oxito", oxito);
	hash_add(&hash, "victor", victor);
	hash_add(&hash, "mariana", mariana);
	hash_add(&hash, "valdeir", valdeir);
	hash_add(&hash, "igor", igor);
	hash_add(&hash, "alan", alan);
	hash_add(&hash, "edgar", edgar);

	struct Node *path = search(&hash, "murilo", "igor");

	while (path != NULL) {
		printf("%s ", path->key);
		path = path->next;
	}
	putchar('\n');

	return 0;
}

void enqueue(struct Queue *q, struct GNode *node) {
	struct QNode *new = malloc(sizeof(struct QNode));
	new->next = NULL;
	new->value = node;

	if (q->first == NULL) q->first = q->last = new;
	else {
		q->last->next = new;
		q->last = new;
		/* q->last = q->last->next = new; */
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
	if (strcmp(start, goal) == 0) return 0;

	int n = 0;
	struct GNode *nstart = hash_getv(hash, start);
	struct GNode *curr;
	struct Queue q;

	nstart->checked = true;
	nstart->parent_name = NULL;
	q.first = q.last = NULL;

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


		       	return first; // todo: change to return a list of the reconstructed path
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

	return -1;
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


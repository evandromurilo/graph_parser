#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lib/hash.h"
#include "lib/m_basics.h"

struct GNode *init_GNode(char *name);
void connect(struct GNode *node, char *str);
int search(struct Hashtable *hash, char *start, char *goal);
struct GNode *dequeue(struct Queue *q);
void enqueue(struct Queue *q, struct GNode *node);

int main(void) {
	struct Hashtable hash;
	hash_init(&hash);

	struct GNode n1;
	n1.name = concat("murilo","");
	n1.checked = false;
	n1.connections = NULL;
	connect(&n1, "iana");
	connect(&n1, "beatriz");
	connect(&n1, "oxito");

	struct GNode n2;
	n2.name = concat("iana","");
	n2.checked = false;
	n2.connections = NULL;
	connect(&n2, "murilo");
	connect(&n2, "josi");

	struct GNode n3;
	n3.name = concat("beatriz","");
	n3.connections = NULL;
	n3.checked = false;

	struct GNode n4;
	n4.name = concat("josi","");
	n4.checked = false;
	n4.connections = NULL;
	connect(&n4, "beatriz");

	struct GNode n5;
	n5.name = concat("oxito","");
	n5.checked = false;
	n5.connections = NULL;
	connect(&n5, "victor");
	connect(&n5, "mariana");

	struct GNode *n6 = init_GNode("victor");
	connect(n6, "igor");
	connect(n6, "valdeir");

	struct GNode *n7 = init_GNode("mariana");
	struct GNode *n8 = init_GNode("valdeir");
	struct GNode *n9 = init_GNode("igor");

	hash_add(&hash, "murilo", &n1);
	hash_add(&hash, "iana", &n2);
	hash_add(&hash, "beatriz", &n3);
	hash_add(&hash, "josi", &n4);
	hash_add(&hash, "oxito", &n5);
	hash_add(&hash, "victor", n6);
	hash_add(&hash, "mariana", n7);
	hash_add(&hash, "valdeir", n8);
	hash_add(&hash, "igor", n9);

	/* printf("%s\n", hash_getv(&hash, "murilo")->connections->str); */
	printf("%d\n", search(&hash, "murilo", "josi"));

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


int search(struct Hashtable *hash, char *start, char *goal) {
	if (strcmp(start, goal) == 0) return 0;

	int n = 0;
	struct GNode *nstart = hash_getv(hash, start);
	struct GNode *curr;
	struct Queue q;

	nstart->checked = true;
	q.first = q.last = NULL;

	enqueue(&q, nstart);

	while ((curr = dequeue(&q)) != NULL) {
		printf("looking at %s\n", curr->name);
		curr->checked = true;
		if (strcmp(curr->name, goal) == 0) return curr->layer;
		++n;

		for (struct StringNode *snd = curr->connections; snd != NULL; snd = snd->next) {
			printf("searching for %s\n", snd->str);
			struct GNode *temp = hash_getv(hash, snd->str);
			printf("got %s\n", temp->name);

			if (!(temp->checked)) {
				enqueue(&q, temp);
				temp->layer = curr->layer + 1;
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

	return new;
}


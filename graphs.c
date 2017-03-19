#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lib/hash.h"
#include "lib/m_basics.h"

int search(struct Hashtable *hash, char *start, char *goal);
struct GNode *dequeue(struct Queue *q);
void enqueue(struct Queue *q, struct GNode *node);

int main(void) {
	struct Hashtable hash;
	hash_init(&hash);

	struct GNode n1;
	n1.name = concat("murilo","");
	n1.checked = false;
	n2.size = 2;
	n1.connections = malloc(sizeof(char**) * 2);
	n1.connections[0] = concat("iana","");
	n1.connections[1] = concat("beatriz","");
	n1.connections[2] = concat("óxito","");

	struct GNode n2;
	n2.name = concat("iana","");
	n2.checked = false;
	n2.size = 2;
	n2.connections = malloc(sizeof(char**) * 2);
	n2.connections[0] = concat("murilo","");
	n2.connections[1] = concat("josi","");

	struct GNode n3;
	n3.name = concat("beatriz","");
	n3.checked = false;
	n3.size = 0;

	struct GNode n4;
	n4.name = concat("josi","");
	n4.checked = false;
	n4.size = 1;
	n4.connections = malloc(sizeof(char**));
	n4.connections[0] = concat("beatriz","");

	struct GNode n5;
	n5.name = concat("óxito","");
	n5.checked = false;
	n5.size = 0;

	hash_add(&hash, "murilo", &n1);
	hash_add(&hash, "iana", &n2);
	hash_add(&hash, "beatriz", &n3);
	hash_add(&hash, "josi", &n4);

	printf("%s\n", hash_getv(&hash, "murilo")->connections[1]);
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
		if (strcmp(curr->name, goal) == 0) return n;
		++n;

		for (int i = 0; i < curr->size; ++i) {
			printf("searching for %s\n", curr->connections[i]);
			struct GNode *temp = hash_getv(hash, curr->connections[i]);
			printf("got %s\n", temp->name);

			if (!(temp->checked)) {
				enqueue(&q, temp);
			}
		}
	}

	return -1;
}

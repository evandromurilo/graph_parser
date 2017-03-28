#ifndef M_LIST_H
#define M_LIST_H

struct LinkedNode {
	struct LinkedNode *next;
	struct LinkedNode *prev;
	void *value;
};

struct LinkedList {
	struct LinkedNode *first;
	struct LinkedNode *last;
	int length;
};

struct LinkedList *init_list();
void append_to_list(struct LinkedList *list, void *value);
void prepend_to_list(struct LinkedList *list, void *value);
void *pop_first(struct LinkedList *list);
void *pop_last(struct LinkedList *list);
bool list_contains(struct LinkedList *list, void *value, bool(*comparison_function)(void *, void *));
int split_linked(struct LinkedList *list, char* str, char *goal);

#endif

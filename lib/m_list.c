#include <stdbool.h>
#include <stdlib.h>
#include "m_basics.h"
#include "m_list.h"

struct LinkedList *init_list() {
	struct LinkedList *list = malloc(sizeof(struct LinkedList));

	list->first = list->last = NULL;
	list->length = 0;

	return list;
}

void append_to_list(struct LinkedList *list, void *value) {
	struct LinkedNode *new = malloc(sizeof(struct LinkedNode));
	new->next = NULL;
	new->prev = list->last;
	new->value = value;

	if (list->length == 0) {
		list->first = list->last = new;
	}
	else {
		list->last->next = new;
		list->last = new;
	}

	++(list->length);
}

void prepend_to_list(struct LinkedList *list, void *value) {
	struct LinkedNode *new = malloc(sizeof(struct LinkedNode));
	new->next = list->first;
	new->prev = NULL;
	new->value = value;

	if (list->length == 0) {
		list->first = list->last = new;
	}
	else {
		list->first->prev = new;
		list->first = new;
	}

	++(list->length);
}

void *pop_first(struct LinkedList *list) {
	if (list->length == 0) return NULL;

	void *value = list->first->value;
	struct LinkedNode *temp = list->first;

	if (list->length == 1) {
		list->first = list->last = NULL;
	}
	else {
		list->first = list->first->next;
		list->first->prev = NULL;
	}

	free(temp);
	--(list->length);

	if (list->length == 0) list->first = list->last = NULL;

	return value;
}

void *pop_last(struct LinkedList *list) {
	if (list->length == 0) return NULL;

	void *value = list->last->value;
	struct LinkedNode *temp = list->last;

	if (list->length == 1) {
		list->last = list->first = NULL;
	}
	else {
		list->last = list->last->prev;
		list->last->next = NULL;
	}

	free(temp);
	--(list->length);

	if (list->length == 0) list->first = list->last = NULL;

	return value;
}

bool list_contains(struct LinkedList *list, void *value, bool(*comparison_function)(void *, void *)) {
	for (struct LinkedNode *curr = list->first; curr != NULL; curr = curr->next) {
		if (comparison_function(curr->value, value)) return true;
	}

	return false;
}

#ifndef MAX_WORD_SIZE
#define MAX_WORD_SIZE 100
#endif
int split_linked(struct LinkedList *list, char* str, char *goal) {
	char word[MAX_WORD_SIZE+1];
	int i, c, total = 0;

	while (*str != '\0') {
		while (char_in_string(*str, goal) && *str != '\0') ++str;

		if (*str == '\0') break;

		for (i = 0; i < MAX_WORD_SIZE && !(char_in_string(*str, goal)) && *str != '\0'; 
				++i, ++str) {
			word[i] = *str;
		}
		word[i] = '\0';

		char* nword = malloc(i);
		strcpy(nword, word);

		append_to_list(list, nword);
		++total;
	}

	return total;
}

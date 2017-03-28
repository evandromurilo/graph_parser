#include "hash.h"

// prepend: sets new->next to head, then swaps head for new
void prepend(struct HashNode **head, struct HashNode *new);

unsigned long hash_function(unsigned char *str) {
	return DEFAULT_HASH_FUNCTION(str);
}

unsigned long sdbm(unsigned char *str) {
	unsigned long hash = 0;
	int c;

	while (c = *str++) {
		hash = c + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}

unsigned long djb2(unsigned char *str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

unsigned long fnv1a(unsigned char *str) {
	unsigned long hash = 14695981039346656037;
	int c;

	while (c = *str++) {
		hash = (hash ^ c) * 1099511628211;
	}

	return hash;
}

struct Hashtable *hash_init() {
	struct Hashtable *table = malloc(sizeof(struct Hashtable));
	table->size = INITIAL_SIZE;
	table->stored = 0;
	table->values = malloc(sizeof(struct HashNode*) * INITIAL_SIZE);

	for (int i = 0; i < INITIAL_SIZE; ++i) table->values[i] = NULL;
	
	return table;
}

void hash_add(struct Hashtable *table, char *key, void *value) {
	double load_factor = (double) table->stored / table->size;
	char* a_key = concat("", key);

	if (DEBUG) printf("load_factor %f\n", load_factor);

	if (load_factor >= 0.7) hash_expand(table);

	struct HashNode *check = hash_getn(table, key);

	if (check != NULL) check->value = value;
	else {
		struct HashNode *new = malloc(sizeof(struct HashNode));
		new->value = value;
		new->key = a_key;

		int i = hash_function(key) % table->size;
		prepend(&(table->values[i]), new);
		++(table->stored);
	}
}

void hash_expand(struct Hashtable *table) {
	if (DEBUG) printf("Expanding hashtable.\n");
	int n_size = next_prime(table->size * 2);
	struct HashNode **n_values = malloc(sizeof(struct HashNode*) * n_size);

	for (int i = 0; i < n_size; ++i) n_values[i] = NULL;

	for (int i = 0; i < table->size; ++i) {
		struct HashNode* curr = table->values[i];

		while (curr != NULL) {
			int ni = hash_function(curr->key) % n_size;
			struct HashNode* next = curr->next;

			prepend(&(n_values[ni]), curr);
			if (DEBUG) printf("%s from %li to %d\n", curr->key, hash_function(curr->key)%table->size, ni);
			curr = next;
		}
	}

	table->size = n_size;
	table->values = n_values;
}

struct HashNode *hash_getn(struct Hashtable *table, char *key) {
	if (DEBUG) printf("Getting node %s.\n", key);
	int i = hash_function(key) % table->size;

	struct HashNode *curr = table->values[i];

	if (DEBUG) if (curr == NULL) printf("there is nothing here at index %d!\n", i);
	while (curr != NULL && strcmp(curr->key, key) != 0) {
		if (DEBUG) printf("its not %s\n", curr->key);
		curr = curr->next;
	}

	return curr;
}

void *hash_getv(struct Hashtable *table, char *key) {
	struct HashNode *curr = hash_getn(table, key);
	return curr == NULL ? NULL : curr->value;
}

void prepend(struct HashNode **head, struct HashNode *new) {
	new->next = *head;
	*head = new;
}

void *hash_remove(struct Hashtable *table, char *key) {
	if (DEBUG) printf("Removing node %s.\n", key);
	int i = hash_function(key) % table->size;
	struct HashNode *prev, *curr;

	prev = NULL;
	curr = table->values[i];
	for (; curr != NULL && strcmp(curr->key, key) != 0; prev = curr, curr = curr->next);

	if (curr == NULL) return NULL;
	else if (prev == NULL) {
		table->values[i] = curr->next;
	}
	else {
		prev->next = curr->next;
	}

	void *value = curr->value;
	free(curr->key);
	free(curr);
	--(table->stored);

	return value;
}

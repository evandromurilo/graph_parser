#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../lib/m_basics.h"

#ifndef DEBUG
#define DEBUG 0
#else
#define DEBUG 1
#endif

#define DEFAULT_HASH_FUNCTION fnv1a
#define INITIAL_SIZE 13

struct Hashtable {
	int size;
	int stored;
	struct HashNode **values;
};

struct HashNode {
	void *value;
	char *key;
	struct HashNode *next;
};

// sdbm: a hash function
// http://www.cse.yorku.ca/~oz/hash.html
unsigned long sdbm(unsigned char *str);

// djb2: a hash function
// http://www.cse.yorku.ca/~oz/hash.html
unsigned long djb2(unsigned char *str);

// fnv1a: a hash function
// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
unsigned long fnv1a(unsigned char *str);

// hash_function: calls the DEFAULT_HASH_FUNCTION
unsigned long hash_function(unsigned char *str);

// hash_init: initializes a hashtable
// returns a pointer to the initialized hashtable
struct Hashtable *hash_init();

// hash_expand: roughly doubles the hashtable size
// called when load_factor >= 0.7
void hash_expand(struct Hashtable *table);

// hash_add: adds key-value to the hashtable
void hash_add(struct Hashtable *table, char *key, void *value);

// hash_getn: gets a node from the hashtable
// returns NULL if key is not present
struct HashNode *hash_getn(struct Hashtable *table, char *key);

// hash_getv: gets a value from the hashtable
// returns 0.0 if key is not present
void *hash_getv(struct Hashtable *table, char *key);

// hash_remove: removes key-value from hashtable
// returns the value removed
void *hash_remove(struct Hashtable *table, char *key);

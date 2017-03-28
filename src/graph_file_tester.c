#include <stdio.h>
#include <stdbool.h>
#include "../lib/hash.h"
#include "../lib/m_basics.h"
#include "../lib/m_list.h"

#define MAX_LINE_SIZE 1000

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

	struct Hashtable *defined = hash_init();
	struct LinkedList *cited = init_list();

	char line[MAX_LINE_SIZE];

	while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
		struct LinkedList *words = init_list();

		split_linked(words, line, " \t\n");
		if (words->length == 0) continue;

		hash_add(defined, pop_first(words), NULL);

		char *curr;
		while (curr = pop_first(words)) append_to_list(cited, curr);
	}

	bool ok = true;
	for (char *curr = pop_first(cited); curr != NULL; curr = pop_first(cited)) {
		if (hash_getn(defined, curr) == NULL) {
			ok = false;
			printf("Node %s cited but not defined!\n", curr);
		}
	}

	fclose(file);

	if (ok) printf("No problems found.\n");
	return 0;
}

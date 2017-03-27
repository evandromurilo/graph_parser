#include <stdio.h>
#include <stdbool.h>
#include "../lib/m_basics.h"
#include "structures.h"

#define MAX_LINE_SIZE 1000

bool present_in(char *str, struct StringList *list);

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Use:: ./graphs 'graphfile.txt'.\n");
		return 1;
	}

	FILE *file = fopen(argv[1], "r");
	if (file == NULL) {
		printf("File '%s' not found.\n", argv[1]);
		return 1;
	}

	struct StringList cited;
	struct StringList defined;
	cited.first = cited.last = defined.first = defined.last = NULL;
	cited.size = defined.size = 0;

	char line[MAX_LINE_SIZE];

	while (fgets(line, MAX_LINE_SIZE, file) != NULL) {
		struct StringList words;
		words.first = words.last = NULL;
		words.size = 0;

		split_linked(&words, line, " \t\n");
		if (words.first == NULL) continue;

		
		append_string(&defined, words.first->str);
		for (struct StringNode *curr = words.first->next; curr != NULL; curr = curr->next) {
			append_string(&cited, curr->str);
		}
	}

	bool ok = true;
	for (struct StringNode *curr = cited.first; curr != NULL; curr = curr->next) {
		if (!present_in(curr->str, &defined)) {
			ok = false;
			printf("Node %s cited but not defined!\n", curr->str);
		}
	}

	fclose(file);

	if (ok) printf("No problems found.\n");
	return 0;
}

bool present_in(char *str, struct StringList *list) {
	for (struct StringNode *curr = list->first; curr != NULL; curr = curr->next) {
		if (strcmp(curr->str, str) == 0) return true;
	}

	return false;
}

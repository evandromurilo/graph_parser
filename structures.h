struct GNode {
	char *name;
	bool checked;
	int size;
	char **connections;
};

struct Hashtable {
	int size;
	int stored;
	struct Node **values;
};

struct Node {
	struct GNode *value;
	char *key;
	struct Node *next;
};

struct QNode {
	struct GNode *value;
	struct QNode *next;
};

struct Queue {
	struct QNode *first;
	struct QNode *last;
};


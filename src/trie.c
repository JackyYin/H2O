#define TRIE_LEVEL_LENGTH (128)

#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

struct h2o_trie_node {
	void *priv;
	bool is_word;
	char *str;
	struct h2o_trie_node *child[TRIE_LEVEL_LENGTH - 1];
};

static struct h2o_trie_node *h2o_trie_node_alloc()
{
	struct h2o_trie_node *newnode = malloc(sizeof(*newnode));
	if (newnode)
		memset(newnode, 0, sizeof(*newnode));
	return newnode;
}

void *h2o_trie_search(struct h2o_trie *trie, char *target, char **found,
		      bool exactly)
{
	if (!target)
		return NULL;

	struct h2o_trie_node *node = trie->root, *next = NULL;
	for (; *target != '\0'; target++) {
		next = node->child[(int)*target - 1];
		if (!next)
			goto NOT_FOUND;
		node = next;
	}

	if (!node->is_word)
		return NULL;

	*found = node->str;
	return node->priv;

NOT_FOUND:
	if (exactly)
		return NULL;

	if (node == trie->root)
		return NULL;

	if (!node->is_word)
		return NULL;

	*found = node->str;
	return node->priv;
}

int h2o_trie_insert(struct h2o_trie *trie, char *target, void *priv)
{
	if (!target)
		return -EIO;

	char *ori = target;

	struct h2o_trie_node **pnode = &trie->root;
	do {
		if (!*pnode) {
			*pnode = h2o_trie_node_alloc();
			if (!*pnode)
				return -ENOMEM;
		}
		pnode = &(*pnode)->child[(int)*target++ - 1];
	} while (*target != '\0');

	if (!*pnode) {
		*pnode = h2o_trie_node_alloc();
		if (!*pnode)
			return -ENOMEM;
	}

	(*pnode)->priv = priv;
	(*pnode)->is_word = 1;
	(*pnode)->str = strdup(ori);
	return 0;
}

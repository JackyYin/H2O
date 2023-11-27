#ifndef _H2O_TRIE_H
#define _H2O_TRIE_H

#include <stdbool.h>

struct h2o_trie_node;

struct h2o_trie {
	struct h2o_trie_node *root;
};

void *h2o_trie_search(struct h2o_trie *trie, char *target, char **matched,
		      bool exactly);
int h2o_trie_insert(struct h2o_trie *trie, char *target, void *priv);

#endif

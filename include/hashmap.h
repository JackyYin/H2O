#ifndef _H2O_HASHMAP_H
#define _H2O_HASHMAP_H

#include <stddef.h>

struct h2o_hashmap_entry;

struct h2o_hashmap {
	size_t capacity;
	size_t length;
	struct h2o_hashmap_entry *buckets;
};

struct h2o_hashmap *h2o_hashmap_init(size_t capacity);
int h2o_hashmap_set(struct h2o_hashmap *hash, char *key, char *value);
char *h2o_hashmap_get(struct h2o_hashmap *hash, char *key);
void h2o_hashmap_uninit(struct h2o_hashmap *hash);

#endif


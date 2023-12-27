#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xxhash.h>

#include "hashmap.h"

struct h2o_hashmap_entry {
	char *key, *value;
};

#define SEED (0x1304759)

struct h2o_hashmap *h2o_hashmap_init(size_t capacity)
{
	struct h2o_hashmap *hash = malloc(sizeof(*hash));

	if (!hash)
		return NULL;

	memset(hash, 0, sizeof(*hash));

	hash->capacity = capacity;
	hash->length = 0;

	hash->buckets = calloc(hash->capacity, sizeof(hash->buckets[0]));
	if (!hash->buckets) {
		free(hash);
		return NULL;
	}
	return hash;
}

static int h2o_hashmap_setentry(struct h2o_hashmap_entry *bucket, size_t cap,
				char *key, char *value, bool dup)
{
	uint64_t hash = XXH64(key, sizeof(key), SEED);
	size_t idx = hash & (cap - 1);

	/* Find occupied slot, if key matched, override value */
	while (bucket[idx].key) {
		if (!strcmp(bucket[idx].key, key)) {
			free(bucket[idx].value);
			bucket[idx].value = dup ? strdup(value) : value;
			return 0;
		}

		if (++idx >= cap)
			idx = 0;
	}

	/* Slot not occupied, use it! */
	bucket[idx].key = dup ? strdup(key) : key;
	bucket[idx].value = dup ? strdup(value) : value;
	return 0;
}

static int h2o_hashmap_expand(struct h2o_hashmap *hash)
{
	struct h2o_hashmap_entry *entry =
	    calloc(hash->capacity * 2, sizeof(hash->buckets[0]));
	if (!entry)
		return -ENOMEM;

	for (size_t i = 0; i < hash->capacity; i++) {
		if (hash->buckets[i].key) {
			h2o_hashmap_setentry(entry, hash->capacity * 2,
					     hash->buckets[i].key,
					     hash->buckets[i].value, 0);
		}
	}

	hash->capacity *= 2;
	free(hash->buckets);
	hash->buckets = entry;
	return 0;
}

int h2o_hashmap_set(struct h2o_hashmap *hash, char *key, char *value)
{
	if (hash->length + 1 >= hash->capacity) {
		if (h2o_hashmap_expand(hash))
			return -ENOMEM;
	}

	h2o_hashmap_setentry(hash->buckets, hash->capacity, key, value, 1);
	hash->length++;
	return 0;
}

char *h2o_hashmap_get(struct h2o_hashmap *hash, char *key)
{
	uint64_t val = XXH64(key, sizeof(key), SEED);

	size_t idx = (val & (hash->capacity - 1));
	/* Find occupied slot, if key matched, return it */
	while (hash->buckets[idx].key) {
		if (!strcmp(key, hash->buckets[idx].key)) {
			return hash->buckets[idx].value;
		}

		if (++idx > hash->capacity)
			idx = 0;
	}
	return NULL;
}

void h2o_hashmap_uninit(struct h2o_hashmap *hash)
{
	for (size_t i = 0; i < hash->capacity; i++) {
		free(hash->buckets[i].key);
		free(hash->buckets[i].value);
	}
	free(hash->buckets);
	free(hash);
}


#ifndef _H2O_HPACK_DYNAMIC_TABLE_H
#define _H2O_HPACK_DYNAMIC_TABLE_H

#include <stddef.h>
#include <stdint.h>

#include "list.h"

typedef struct hpack_dynamic_table_entry {
	struct list_head list;
	size_t namelen;
	size_t valuelen;
	uint8_t buf[];
} hdt_entry;

typedef struct hpack_dynamic_table {
	size_t capacity;
	size_t footprint;
	struct list_head head;
} hdt;

#define hdt_for_each_entry_reverse(pos, head, member)                          \
	list_for_each_entry_reverse(pos, head, member)

void hdt_init(hdt *tbl, size_t size);
int hdt_push(hdt *tbl, char *name, size_t namelen, char *value,
	     size_t valuelen);
void hdt_pop(hdt *tbl);
int hdt_peek_nth(hdt *tbl, char *name, size_t *namelen, char *value,
		 size_t *valuelen, int index);
int hdt_update_size(hdt *tbl, size_t size);
#endif

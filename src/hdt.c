#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "hdt.h"

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{

	new->next = head;
	new->prev = head->prev;
	head->prev = new;
	new->prev->next = new;
}

static inline void list_del(struct list_head *prev, struct list_head *next)
{
	prev->next = next;
	next->prev = prev;
}

static void hdt_free_entry(hdt *tbl, struct list_head *node)
{
	hdt_entry *data = container_of(hdt_entry, list, node);
	tbl->footprint -= (sizeof(hdt_entry) + data->namelen + data->valuelen);
	list_del(node->prev, node->next);
	free(data);
}

void hdt_init(hdt *tbl, size_t size)
{
	tbl->capacity = size;
	tbl->footprint = 0;
	INIT_LIST_HEAD(&tbl->head);
}

void hdt_pop(hdt *tbl)
{
	struct list_head *cur = tbl->head.next;
	if (cur != &tbl->head)
		hdt_free_entry(tbl, cur);
}

int hdt_push(hdt *tbl, char *name, size_t namelen, char *value, size_t valuelen)
{
	size_t entrysize = sizeof(hdt_entry) + namelen + valuelen;
	while (tbl->footprint + entrysize >= tbl->capacity) {
		struct list_head *cur = tbl->head.next;
		if (cur == &tbl->head)
			return -E2BIG;
		hdt_free_entry(tbl, cur);
	}

	hdt_entry *entry = malloc(entrysize);
	entry->namelen = namelen;
	entry->valuelen = valuelen;
	memcpy(entry->buf, name, namelen);
	memcpy(entry->buf + namelen, value, valuelen);
	list_add_tail(&entry->list, &tbl->head);
	tbl->footprint += entrysize;
	return 0;
}

int hdt_peek_nth(hdt *tbl, char *name, size_t *namelen, char *value,
		 size_t *valuelen, int index)
{
	if (index < 0)
		return -EINVAL;

	hdt_entry *data = NULL;
	hdt_for_each_entry_reverse(data, &tbl->head, list)
	{
		if (!index--) {
			if (namelen && data->namelen <= *namelen) {
				*namelen = data->namelen;
				memcpy(name, data->buf, *namelen);
			}
			if (valuelen && data->valuelen <= *valuelen) {
				*valuelen = data->valuelen;
				memcpy(value, data->buf + data->namelen,
				       *valuelen);
			}
			return 0;
		}
	}
	return -ENOENT;
}

int hdt_update_size(hdt *tbl, size_t size)
{
	tbl->capacity = size;
	return 0;
}

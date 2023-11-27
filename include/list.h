#ifndef _H2O_LIST_H
#define _H2O_LIST_H

struct list_head {
	struct list_head *next, *prev;
};

#define container_of(type, member, pointer)                                    \
	({                                                                     \
		type *ptype = (type *)0x00;                                    \
		(type *)((void *)(pointer) - (size_t)&ptype->member);          \
	})

#define list_for_each_entry(pos, head, member)                                 \
	for (pos = container_of(typeof(*pos), member, (head)->next);           \
	     &pos->member != (head);                                           \
	     pos = container_of(typeof(*pos), member, pos->member.next))

#define list_for_each_entry_reverse(pos, head, member)                         \
	for (pos = container_of(typeof(*pos), member, (head)->prev);           \
	     &pos->member != (head);                                           \
	     pos = container_of(typeof(*pos), member, pos->member.prev))

#endif

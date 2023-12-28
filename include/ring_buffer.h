#ifndef _H2O_RING_BUF_H
#define _H2O_RING_BUF_H

#include <assert.h>
#include <errno.h>
#include <string.h>

#define DECLARE_RINGBUF_T(rb_name, type_name, element_size)                    \
	static_assert((element_size & (element_size - 1)) == 0,                \
		      "ring buffer size should be power of 2");                \
	struct rb_name {                                                       \
		int head, tail;                                                \
		type_name eles[element_size];                                  \
	};                                                                     \
	__attribute__((unused)) static void rb_name##_init(struct rb_name *rb) \
	{                                                                      \
		memset(rb, 0, sizeof(*rb));                                    \
	}                                                                      \
	static void rb_name##_advance(int *ptr)                                \
	{                                                                      \
		*ptr = (*ptr + 1) & (element_size - 1);                        \
	}                                                                      \
	static int rb_name##_full(struct rb_name *rb)                          \
	{                                                                      \
		return (((rb->tail + 1) & (element_size - 1)) == rb->head);    \
	}                                                                      \
	static int rb_name##_empty(struct rb_name *rb)                         \
	{                                                                      \
		return (rb->tail == rb->head);                                 \
	}                                                                      \
	__attribute__((unused)) static int rb_name##_push(struct rb_name *rb,  \
							  type_name *pt)       \
	{                                                                      \
		if (rb_name##_full(rb))                                        \
			return -E2BIG;                                         \
		memcpy(&rb->eles[rb->tail], pt, sizeof(type_name));            \
		rb_name##_advance(&rb->tail);                                  \
		return 0;                                                      \
	}                                                                      \
	__attribute__((unused)) static type_name *rb_name##_peek(              \
	    struct rb_name *rb)                                                \
	{                                                                      \
		if (rb_name##_empty(rb))                                       \
			return NULL;                                           \
		type_name *pt = &rb->eles[rb->head];                           \
		return pt;                                                     \
	}                                                                      \
	__attribute__((unused)) static type_name *rb_name##_pop(               \
	    struct rb_name *rb)                                                \
	{                                                                      \
		if (rb_name##_empty(rb))                                       \
			return NULL;                                           \
		type_name *pt = &rb->eles[rb->head];                           \
		rb_name##_advance(&rb->head);                                  \
		return pt;                                                     \
	}

#endif

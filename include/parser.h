#ifndef _H2O_PARSER_H
#define _H2O_PARSER_H

#include "lexer.h"
#include "ring_buffer.h"

enum parser_item_type {
	PARSER_ITEM_TYPE_KV,
	PARSER_ITEM_TYPE_SECTION,
	PARSER_ITEM_TYPE_SECTION_END
};

struct parser_item {
	enum parser_item_type type;
	char key[124];
	char val[128];
};

DECLARE_RINGBUF_T(parser_ring_buffer, struct parser_item, 8)

struct h2o_parser {
	void *(*handler)(struct h2o_parser *);
	struct h2o_lexer lexer;
	struct lexer_ring_buffer rb;
	struct parser_ring_buffer items;
};

#define init_parser_ring_buffer(parser)                                        \
	do {                                                                   \
		lexer_ring_buffer_init(&(parser)->rb);                         \
		parser_ring_buffer_init(&(parser)->items);                     \
	} while (0);

struct parser_item *parser_get_item(struct h2o_parser *parser);
#endif

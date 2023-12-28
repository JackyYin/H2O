#include "parser.h"

struct parser_item *parser_get_item(struct h2o_parser *parser)
{
	struct parser_item *item = NULL;
	while (parser->handler) {
		if ((item = parser_ring_buffer_pop(&parser->items)))
			return item;

		parser->handler = parser->handler(parser);
	}
	return parser_ring_buffer_pop(&parser->items);
}

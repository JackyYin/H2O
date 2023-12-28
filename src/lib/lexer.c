#include <stdint.h>

#include "lexer.h"

void set_lexer_addr_range(struct h2o_lexer *lexer, uint8_t *begin, uint8_t *end)
{
	lexer->cur = lexer->start = begin;
	lexer->end = end;
}

void emit(struct h2o_lexer *lexer, enum lexer_token_type type)
{
	struct lexer_token tok = {.type = type,
				  .buf = (char *)lexer->start,
				  .buflen =
				      (size_t)(lexer->cur - lexer->start)};
	lexer_ring_buffer_push(&lexer->rb, &tok);
	lexer->start = lexer->cur;
}

struct lexer_token *lexer_get_token(struct h2o_lexer *lexer)
{
	struct lexer_token *tok;
	while (lexer->handler) {
		if ((tok = lexer_ring_buffer_pop(&lexer->rb)))
			return tok;

		lexer->handler = lexer->handler(lexer);
	}
	return lexer_ring_buffer_pop(&lexer->rb);
}


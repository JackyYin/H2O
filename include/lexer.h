#ifndef _H2O_LEXER_H
#define _H2O_LEXER_H

#include <stddef.h>
#include <stdint.h>

#include "ring_buffer.h"

enum lexer_token_type {
	TOKEN_TYPE_STRING,
	TOKEN_TYPE_NEWLINE,
	TOKEN_TYPE_OPEN_CURLY_BRACKET,
	TOKEN_TYPE_CLOSE_CURLY_BRACKET,
	TOKEN_TYPE_UNKNOWN
};

struct lexer_token {
	enum lexer_token_type type;
	char *buf;
	size_t buflen;
};

DECLARE_RINGBUF_T(lexer_ring_buffer, struct lexer_token, 8)

struct h2o_lexer {
	void *(*handler)(struct h2o_lexer *);
	uint8_t *cur, *start, *end;
	struct lexer_ring_buffer rb;
};

#define ignore(lexer) lexer->start = lexer->cur

#define init_lexer_ring_buffer(lexer)                                          \
	do {                                                                   \
		lexer_ring_buffer_init(&(lexer)->rb);                          \
	} while (0);

static inline uint8_t lexer_next_char(struct h2o_lexer *lexer)
{
	return (lexer->cur >= lexer->end) ? 0 : *lexer->cur++;
}

void set_lexer_addr_range(struct h2o_lexer *lexer, uint8_t *begin,
			  uint8_t *end);
void emit(struct h2o_lexer *lexer, enum lexer_token_type type);
struct lexer_token *lexer_get_token(struct h2o_lexer *lexer);

#endif

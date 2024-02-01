#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "config.h"
#include "hashmap.h"
#include "lexer.h"
#include "parser.h"

static inline int isstring(uint8_t c) { return isprint((int)c) && c != ' '; }

static void *lexer_handler(struct h2o_lexer *lexer);
static void *lexer_string_handler(struct h2o_lexer *lexer)
{
	uint8_t c;
	do {
		c = lexer_next_char(lexer);
	} while (isstring(c));
	lexer->cur--;
	emit(lexer, TOKEN_TYPE_STRING);
	return c == 0 ? NULL : lexer_handler;
}

static void *lexer_handler(struct h2o_lexer *lexer)
{
	while (1) {
		uint8_t c = lexer_next_char(lexer);
		if (c == 0)
			break;

		if (c == '#') {
			char *newline = strchr((char *)lexer->cur, '\n');
			if (!newline)
				return NULL;

			lexer->cur = (uint8_t *)(newline + 1);
			ignore(lexer);
			continue;
		}

		if (c == '\n') {
			emit(lexer, TOKEN_TYPE_NEWLINE);
			return lexer_handler;
		}

		/*
		 * char : \t, \n, \s, \f, \r
		 */
		if (isspace(c)) {
			ignore(lexer);
			return lexer_handler;
		}

		if (isprint((int)c)) // treat it as a string
			return lexer_string_handler;
	}
	return NULL;
}

static void *parser_handler(struct h2o_parser *parser);
static void *parser_kv_handler(struct h2o_parser *parser)
{
	struct lexer_token *key = NULL, *val = NULL;

	if (!(val = lexer_ring_buffer_peek(&parser->rb)) ||
	    val->type != TOKEN_TYPE_STRING)
		return parser_handler;
	lexer_ring_buffer_pop(&parser->rb);

	while ((key = lexer_ring_buffer_peek(&parser->rb)) &&
	       key->type == TOKEN_TYPE_STRING) {
		struct parser_item item = {.type = PARSER_ITEM_TYPE_KV};
		memcpy(item.key, key->buf, key->buflen);
		memcpy(item.val, val->buf, val->buflen);
		parser_ring_buffer_push(&parser->items, &item);
		lexer_ring_buffer_pop(&parser->rb);
	}
	return parser_handler;
}

static void *parser_handler(struct h2o_parser *parser)
{
	struct lexer_token *tok = NULL;
	while ((tok = lexer_get_token(&parser->lexer))) {
		switch (tok->type) {
		case TOKEN_TYPE_STRING:
			lexer_ring_buffer_push(&parser->rb, tok);
			return parser_handler;
		case TOKEN_TYPE_NEWLINE:
			return parser_kv_handler;
		default:
			return NULL;
		}
	}
	return NULL;
}

static void do_parse(struct h2o_parser *parser)
{
	struct parser_item *item;
	while ((item = parser_get_item(parser))) {
		switch (item->type) {
		case PARSER_ITEM_TYPE_KV:
			printf("{\"%s\", \"%s\"},\n", item->key, item->val);
		default:
			break;
		}
	}
}

static int get_file_mmap_region(const char *path, void **start, size_t *size)
{
	int fd = open(path, O_RDONLY);
	if (fd == -1)
		return -errno;

	struct stat st;
	int ret = fstat(fd, &st);
	if (ret == -1)
		goto CLOSE;

	void *addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED)
		goto CLOSE;

	*start = addr;
	*size = st.st_size;
	close(fd);
	return 0;

CLOSE:
	close(fd);
	return -errno;
}

int main(int argc, char **argv)
{
	int ret;
	void *mmap_start;
	size_t mmap_size;
	struct h2o_lexer lexer = {.handler = lexer_handler};
	struct h2o_parser parser = {.handler = parser_handler};

	if (argc < 2) {
		fprintf(stderr, "Usage: %s PATH_TO_MIME_TYPES_FILE\n", argv[0]);
		exit(-1);
	}

	ret = get_file_mmap_region(argv[1], &mmap_start, &mmap_size);
	if (ret) {
		fprintf(stderr, "Failed to get mmaped region of file: %s\n",
			argv[0]);
		exit(-1);
	}

	init_lexer_ring_buffer(&lexer);
	set_lexer_addr_range(&lexer, mmap_start, mmap_start + mmap_size);
	init_parser_ring_buffer(&parser);
	parser.lexer = lexer;

	printf("#ifndef _H2O_MIME_H\n");
	printf("#define _H2O_MIME_H\n");
	printf("char *mime_types[][2] = {\n");
	do_parse(&parser);
	printf("};\n");
	printf("#endif\n");

	munmap(mmap_start, mmap_size);
	return 0;
}

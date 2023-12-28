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
#include "h2o.h"
#include "hashmap.h"
#include "lexer.h"
#include "parser.h"
#include "ring_buffer.h"
#include "router.h"

static struct h2o_config default_config = {
    .listen_on = 8080,
    .config_file_path = "./h2o.conf",
    .config_file_addr_begin = NULL,
    .config_file_addr_end = NULL,
};

static inline int isstring(uint8_t c)
{
	return isprint((int)c) && c != ' ' && c != '{' && c != '}';
}

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
		if (c == '\0')
			break;

		if (c == '\n') {
			emit(lexer, TOKEN_TYPE_NEWLINE);
			return lexer_handler;
		}
		/*
		 * char : \f, \n, \r, \t, \v
		 */
		if (isspace((int)c)) {
			ignore(lexer);
			return lexer_handler;
		}

		if (c == '{') {
			emit(lexer, TOKEN_TYPE_OPEN_CURLY_BRACKET);
			return lexer_handler;
		}

		if (c == '}') {
			emit(lexer, TOKEN_TYPE_CLOSE_CURLY_BRACKET);
			return lexer_handler;
		}

		if (isprint((int)c)) // treat it as a string
			return lexer_string_handler;
	}
	return NULL;
}

static void *parser_handler(struct h2o_parser *parser);
static void *parser_section_prefix(struct h2o_parser *parser)
{
	struct parser_item item = {.type = PARSER_ITEM_TYPE_SECTION};
	struct lexer_token *key = NULL, *val = NULL;

	if (!(key = lexer_ring_buffer_pop(&parser->rb)) ||
	    key->type != TOKEN_TYPE_STRING)
		return NULL;

	if (!(val = lexer_ring_buffer_pop(&parser->rb)) ||
	    val->type != TOKEN_TYPE_STRING) {
		return NULL;
	}

	memcpy(item.key, key->buf, key->buflen);
	memcpy(item.val, val->buf, val->buflen);
	parser_ring_buffer_push(&parser->items, &item);
	return parser_handler;
}

static void *parser_kv_handler(struct h2o_parser *parser)
{
	struct parser_item item = {.type = PARSER_ITEM_TYPE_KV};
	struct lexer_token *key = NULL, *val = NULL;

	if (!(key = lexer_ring_buffer_peek(&parser->rb)) ||
	    key->type != TOKEN_TYPE_STRING)
		return parser_handler;
	lexer_ring_buffer_pop(&parser->rb);

	if (!(val = lexer_ring_buffer_peek(&parser->rb)) ||
	    val->type != TOKEN_TYPE_STRING) {
		lexer_ring_buffer_push(&parser->rb, key);
		return parser_handler;
	}
	lexer_ring_buffer_pop(&parser->rb);

	memcpy(item.key, key->buf, key->buflen);
	memcpy(item.val, val->buf, val->buflen);
	parser_ring_buffer_push(&parser->items, &item);
	return parser_handler;
}

static void *parser_handler(struct h2o_parser *parser)
{
	struct lexer_token *tok = NULL;
	while ((tok = lexer_get_token(&parser->lexer))) {
		switch (tok->type) {
		case TOKEN_TYPE_NEWLINE:
			return parser_kv_handler;
		case TOKEN_TYPE_OPEN_CURLY_BRACKET:
			return parser_section_prefix;
		case TOKEN_TYPE_CLOSE_CURLY_BRACKET:
			struct parser_item item = {
			    .type = PARSER_ITEM_TYPE_SECTION_END};
			parser_ring_buffer_push(&parser->items, &item);
			return parser_handler;
		case TOKEN_TYPE_STRING:
			lexer_ring_buffer_push(&parser->rb, tok);
			return parser_handler;
		default:
			return NULL;
		}
	}
	return NULL;
}

static int parse_location(struct h2o *h, struct h2o_config *conf,
			  struct h2o_parser *parser, struct parser_item *item)
{
	char *path = item->val;
	struct h2o_router *router = NULL;
	struct h2o_hashmap *map = h2o_hashmap_init(16);

	if (!map)
		return -ENOMEM;

	while ((item = parser_get_item(parser))) {
		switch (item->type) {
		case PARSER_ITEM_TYPE_KV:
			if (map) {
				h2o_hashmap_set(map, item->key, item->val);
			}
			break;
		case PARSER_ITEM_TYPE_SECTION:
			printf("WARNING: recursively section encountered!\n");
			break;
		case PARSER_ITEM_TYPE_SECTION_END:
			if (map && (router = h2o_init_router(map))) {
				h2o_trie_insert(&h->url_map_trie, path,
						(void *)router);
				h2o_hashmap_uninit(map);
			}
			return 0;
		}
	}

	printf("WARNING: section start without ending\n");
	return -ENOENT;
}

static int do_parse(struct h2o *h, struct h2o_config *conf,
		    struct h2o_parser *parser)
{
	struct parser_item *item;
	while ((item = parser_get_item(parser))) {
		switch (item->type) {
		case PARSER_ITEM_TYPE_KV:
			if (strcmp(item->key, "listen") == 0) {
				conf->listen_on = atoi(item->val);
			}
			break;
		case PARSER_ITEM_TYPE_SECTION:
			if (strcmp(item->key, "location") == 0) {
				parse_location(h, conf, parser, item);
			}
		default:
			break;
		}
	}
	return 0;
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

struct h2o_config *h2o_get_default_config() { return &default_config; }

int h2o_init_config(struct h2o *h, struct h2o_config *conf)
{
	if (conf->config_file_path) {
		void *mmap_start;
		size_t mmap_size;
		struct h2o_lexer lexer = {.handler = lexer_handler};
		struct h2o_parser parser = {.handler = parser_handler};

		int ret = get_file_mmap_region(conf->config_file_path,
					       &mmap_start, &mmap_size);
		if (ret)
			return ret;

		init_lexer_ring_buffer(&lexer);
		init_parser_ring_buffer(&parser);
		set_lexer_addr_range(&lexer, mmap_start,
				     mmap_start + mmap_size);
		parser.lexer = lexer;

		ret = do_parse(h, conf, &parser);
		munmap(mmap_start, mmap_size);
		return ret;
	}
	return 0;
}

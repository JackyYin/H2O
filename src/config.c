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
#include "ring_buffer.h"
#include "router.h"

static struct h2o_config default_config = {
    .listen_on = 8080,
    .config_file_path = "./h2o.conf",
    .config_file_addr_begin = NULL,
    .config_file_addr_end = NULL,
};

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

enum parser_item_type {
	ITEM_TYPE_KV,
	ITEM_TYPE_SECTION,
	ITEM_TYPE_SECTION_END
};

struct parser_item {
	enum parser_item_type type;
	char key[64];
	char val[64];
};

DECLARE_RINGBUF_T(lexer_ring_buffer, struct lexer_token, 8)
DECLARE_RINGBUF_T(parser_ring_buffer, struct parser_item, 8)

struct h2o_lexer {
	void *(*handler)(struct h2o_lexer *);
	uint8_t *cur, *start, *end;
	struct lexer_ring_buffer rb;
};

struct h2o_parser {
	void *(*handler)(struct h2o_parser *);
	struct h2o_lexer lexer;
	struct lexer_ring_buffer rb;
	struct parser_ring_buffer items;
};

static int open_and_map_config_file(struct h2o_config *conf)
{
	int fd = open(conf->config_file_path, O_RDONLY);
	if (fd == -1)
		return -errno;

	struct stat st;
	int ret = fstat(fd, &st);
	if (ret == -1)
		goto CLOSE;

	void *addr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED)
		goto CLOSE;

	conf->config_file_addr_begin = addr;
	conf->config_file_addr_end = addr + st.st_size;
	close(fd);
	return 0;

CLOSE:
	close(fd);
	return -errno;
}

static int isstring(uint8_t c)
{
	return isprint((int)c) && !isspace((int)c) && c != 0x7b && c != 0x7d;
}

static uint8_t lexer_next_char(struct h2o_lexer *lexer)
{
	if (lexer->cur >= lexer->end)
		return 0;

	return *lexer->cur++;
}

static void emit(struct h2o_lexer *lexer, enum lexer_token_type type)
{
	struct lexer_token tok = {.type = type,
				  .buf = (char *)lexer->start,
				  .buflen =
				      (size_t)(lexer->cur - lexer->start)};
	lexer_ring_buffer_push(&lexer->rb, &tok);
	lexer->start = lexer->cur;
}

static void ignore(struct h2o_lexer *lexer) { lexer->start = lexer->cur; }

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
		if (c == 0) // \0
			break;

		/*
		 * char : \n
		 */
		if (c == 0x0a) {
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
		/*
		 * char : {
		 */
		if (c == 0x7b) {
			emit(lexer, TOKEN_TYPE_OPEN_CURLY_BRACKET);
			return lexer_handler;
		}
		/*
		 * char : {
		 */
		if (c == 0x7d) {
			emit(lexer, TOKEN_TYPE_CLOSE_CURLY_BRACKET);
			return lexer_handler;
		}

		if (isprint((int)c)) // treat it as a string
			return lexer_string_handler;
	}
	return NULL;
}

static struct lexer_token *lexer_get_token(struct h2o_lexer *lexer)
{
	struct lexer_token *tok;
	while (lexer->handler) {
		if ((tok = lexer_ring_buffer_pop(&lexer->rb)))
			return tok;

		lexer->handler = lexer->handler(lexer);
	}
	return lexer_ring_buffer_pop(&lexer->rb);
}

static void *parser_handler(struct h2o_parser *parser);

static void *parser_section_prefix(struct h2o_parser *parser)
{
	struct parser_item item = {.type = ITEM_TYPE_SECTION};
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
	struct parser_item item = {.type = ITEM_TYPE_KV};
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
			struct parser_item item = {.type =
						       ITEM_TYPE_SECTION_END};
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

static struct parser_item *parser_get_item(struct h2o_parser *parser)
{
	struct parser_item *item = NULL;
	while (parser->handler) {
		if ((item = parser_ring_buffer_pop(&parser->items)))
			return item;

		parser->handler = parser->handler(parser);
	}
	return parser_ring_buffer_pop(&parser->items);
}

static int parse_location(struct h2o *h, struct h2o_config *conf,
			  struct h2o_parser *parser, struct parser_item *item)
{
	char *path = item->val;
	struct h2o_router *router = NULL;
	struct h2o_hashmap *map = h2o_hashmap_init();

	if (!map)
		return -ENOMEM;

	while ((item = parser_get_item(parser))) {
		switch (item->type) {
		case ITEM_TYPE_KV:
			if (map) {
				h2o_hashmap_set(map, item->key, item->val);
			}
			break;
		case ITEM_TYPE_SECTION:
			printf("WARNING: recursively section encountered!\n");
			break;
		case ITEM_TYPE_SECTION_END:
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

static int parse_config_file(struct h2o *h, struct h2o_config *conf)
{
	struct h2o_parser parser = {
	    .handler = parser_handler,
	    .lexer = {
		.handler = lexer_handler,
		.cur = (uint8_t *)conf->config_file_addr_begin,
		.start = (uint8_t *)conf->config_file_addr_begin,
		.end = (uint8_t *)conf->config_file_addr_end,
	    }};
	lexer_ring_buffer_init(&parser.rb);
	lexer_ring_buffer_init(&parser.lexer.rb);
	parser_ring_buffer_init(&parser.items);

	struct parser_item *item;
	while ((item = parser_get_item(&parser))) {
		switch (item->type) {
		case ITEM_TYPE_KV:
			if (strcmp(item->key, "listen") == 0) {
				conf->listen_on = atoi(item->val);
			}
			break;
		case ITEM_TYPE_SECTION:
			if (strcmp(item->key, "location") == 0) {
				parse_location(h, conf, &parser, item);
			}
		default:
			break;
		}
	}
	return 0;
}

static int try_load_config_file(struct h2o *h, struct h2o_config *conf)
{
	int ret = open_and_map_config_file(conf);
	if (ret)
		return ret;

	ret = parse_config_file(h, conf);
	if (ret)
		return ret;

	return 0;
}

struct h2o_config *h2o_get_default_config() { return &default_config; }

int h2o_init_config(struct h2o *h, struct h2o_config *conf)
{
	if (conf->config_file_path) {
		if (try_load_config_file(h, conf))
			printf("WARNING: failed to load config from "
			       "config_file: %s\n",
			       conf->config_file_path);
	}
	return 0;
}
